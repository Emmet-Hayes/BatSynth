#pragma once
#include "NoteToColor.h"
#include <cmath>

template <typename SampleType>
class AudioBufferQueue 
{
public:
	static constexpr size_t order = 10;
	static constexpr size_t bufferSize = 1U << order;
	static constexpr size_t numBuffers = 5;

	void push(const SampleType* dataToPush, size_t numSamples) 
	{
		jassert(numSamples <= bufferSize);
		int start1, size1, start2, size2;
		abstractFifo.prepareToWrite(1, start1, size1, start2, size2);
		jassert(size1 <= 1);
		jassert(size2 == 0);
		if (size1 > 0)
			FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, (int)jmin(bufferSize, numSamples));
		abstractFifo.finishedWrite(size1);
	}

	void pop(SampleType* outputBuffer) 
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToRead(1, start1, size1, start2, size2);
		jassert(size1 <= 1);
		jassert(size2 == 0);
		if (size1 > 0) FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)bufferSize);
		abstractFifo.finishedRead(size1);
	}

private:
	AbstractFifo abstractFifo{ numBuffers };
	std::array<std::array<SampleType, bufferSize>, numBuffers> buffers;
};

template <typename SampleType>
class ScopeComponent : public NoteColorListener
	                 , public juce::Component
		             , private Timer
{
public:
	ScopeComponent(AudioBufferQueue<SampleType>& queueToUse)
	:   audioBufferQueue(queueToUse) 
	{
		buffer.fill(SampleType(0));
		setFramesPerSecond(60);
	}

	void noteColorChanged(NoteColors newColor) override 
	{
		currentNoteColor = newColor;
	}

	void setFramesPerSecond(int framesPerSecond) 
	{
		jassert(framesPerSecond > 0 && framesPerSecond < 1000);
		startTimerHz(framesPerSecond);
	}

	void paint(Graphics& g) override 
	{
		g.setColour(noteToColorMap[currentNoteColor]);
		auto area = getLocalBounds();
		auto h = (SampleType)area.getHeight();
		auto w = (SampleType)area.getWidth();

		auto scopeRect = juce::Rectangle<SampleType>{ SampleType(0), SampleType(0), w, h / 2 };
		plot(buffer.data(), buffer.size(), g, scopeRect, SampleType(1), h / 4);
		auto spectrumRect = juce::Rectangle<SampleType>{ SampleType(0), h / 2, w, h / 2 };
		plot(spectrumData.data(), spectrumData.size() / 4, g, spectrumRect);
	}

	void resized() override {}

private:
	NoteColors currentNoteColor = NoteColors::WHITE;
	AudioBufferQueue<SampleType>& audioBufferQueue;
	std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
	juce::dsp::FFT fft{ AudioBufferQueue<SampleType>::order };
	juce::dsp::WindowingFunction<SampleType> windowFun{ (size_t)fft.getSize(),
		                             juce::dsp::WindowingFunction<SampleType>::hann };
	std::array<SampleType, 2 * AudioBufferQueue<SampleType>::bufferSize> spectrumData;

	void timerCallback() override 
	{
		audioBufferQueue.pop(buffer.data());
		FloatVectorOperations::copy(spectrumData.data(), buffer.data(), (int)buffer.size());
		jassert(spectrumData.size() == 2 * (size_t)fft.getSize());
		windowFun.multiplyWithWindowingTable(spectrumData.data(), (size_t)fft.getSize());
		fft.performFrequencyOnlyForwardTransform(spectrumData.data());
		static constexpr auto mindB = SampleType(-160);
		static constexpr auto maxdB = SampleType(0);
		for (auto& s : spectrumData)
			s = jmap(jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(s) - juce::Decibels::gainToDecibels(SampleType((size_t)fft.getSize()))), mindB, maxdB, SampleType(0), SampleType(1));
		repaint();
	}

	static void plot(const SampleType* data, size_t numSamples, Graphics& g, juce::Rectangle<SampleType> rect,
		SampleType scale = SampleType(0.7), SampleType offset = SampleType(0)) 
	{
		auto w = rect.getWidth();
		auto h = rect.getHeight();
		auto right = rect.getRight();
		auto center = rect.getBottom() - offset;
		auto gain = h * scale;
		for (size_t i = 1; i < numSamples; ++i)
			g.drawLine({ jmap(SampleType(i - 1), SampleType(0), SampleType(numSamples - 1), SampleType(right - w),
				SampleType(right)), center - gain * data[i - 1], jmap(SampleType(i), SampleType(0),
				SampleType(numSamples - 1), SampleType(right - w), SampleType(right)), center - gain * data[i] });
	}
};

template <typename SampleType>
class ScopeDataCollector 
{
public:
	ScopeDataCollector(AudioBufferQueue<SampleType>& queueToUse)
	:   audioBufferQueue(queueToUse)
	{}

	void addListener(NoteColorListener* listener) 
	{
		listeners.push_back(listener);
	}

	void removeAllListeners() 
	{
		for (int i = 0; i < listeners.size(); ++i)
			listeners.pop_back();
	}

	void process(const SampleType* data, size_t numSamples, float minFrequency) 
	{
		size_t index = 0;
		
		if (state == State::waitingForTrigger) 
		{
			while (index++ < numSamples) 
			{
				auto currentSample = *data++;
				if (fabs(currentSample) >= triggerLevel && prevSample < triggerLevel) 
				{
					numCollected = 0;
					state = State::collecting;
					break;
				}
				prevSample = currentSample;
			}
		}

		if (state == State::collecting) 
		{
			while (index++ < numSamples) 
			{
				buffer[numCollected++] = *data++;
				if (numCollected == buffer.size()) 
				{
					audioBufferQueue.push(buffer.data(), buffer.size());
					state = State::waitingForTrigger;
					prevSample = SampleType(100);
					break;
				}
			}
		}

		for (NoteColorListener* listener : listeners)
			if (listener != nullptr)
				listener->noteColorChanged(frequencyToColor(minFrequency));
	}

private:
	std::vector<NoteColorListener*> listeners;
	AudioBufferQueue<SampleType>& audioBufferQueue;
	std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
	size_t numCollected;
	SampleType prevSample = SampleType(100);
	static constexpr auto triggerLevel = SampleType(0.01);
	enum class State { waitingForTrigger, collecting } state { State::waitingForTrigger };
};
