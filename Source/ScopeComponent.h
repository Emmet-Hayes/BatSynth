/* ScopeComponent.h
 
  AudioBufferQueue and ScopeComponent are responsible for rendering the
 oscilloscope in real time (or as close to as possible) as audio is being generated.
 All classes are capable of switching between single and double precision.
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>
//==============================================================================
template <typename SampleType>
class AudioBufferQueue 
{
public:
    // some global variables for making the buffers
    static constexpr size_t order = 10; //designates size of the fft window and number of points
    static constexpr size_t bufferSize = 1U << order;
    static constexpr size_t numBuffers = 5;

    //add data to the buffer with given number of samples
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

    //remove data from the output buffer
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
    AbstractFifo abstractFifo{ numBuffers }; //a first-in first-out sequenced data type
    std::array<std::array<SampleType, bufferSize>, numBuffers> buffers; //3d array holding audio rep type (float), and size of each buffer
};

//This class is initialized with the buffer
template <typename SampleType>
class ScopeDataCollector 
{
public:
  //initialize with class above
    ScopeDataCollector(AudioBufferQueue<SampleType>& queueToUse)
    :   audioBufferQueue(queueToUse)
    {}

    //represent the audio data visually with this processing <3
    void process(const SampleType* data, size_t numSamples) 
    {
        size_t index = 0;
        //starts here, if sample over trigger level it collects.
        if (state == State::waitingForTrigger) 
        {
            while (index++ < numSamples) 
            {
                auto currentSample = *data++; //get the data and increment to the next
                //if the triggerlevel is newly hit
                if (fabs(currentSample) >= triggerLevel && prevSample < triggerLevel) 
                {
                    numCollected = 0; //initializer
                    state = State::collecting;
                    break;
                }
                prevSample = currentSample; //zeros out our silence
            }
        }

        //fills the buffer with data if audio is being collected
        if (state == State::collecting) 
        {
            while (index++ < numSamples) 
            {
                buffer[numCollected++] = *data++; //copy data to buffer, move to the next slots
                if (numCollected == buffer.size()) 
                { //if we got to the end of the buffer
                    audioBufferQueue.push(buffer.data(), buffer.size()); //put it in the queue
                    state = State::waitingForTrigger; //set back
                    prevSample = SampleType(100); //reinitialize
                    break;
                }
            }
        }
    }

private:
    AudioBufferQueue<SampleType>& audioBufferQueue; //see class above
    std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer; //holds raw data
    size_t numCollected; //indexer for buffer array
    SampleType prevSample = SampleType(100);
    static constexpr auto triggerLevel = SampleType(0.01); //threshold of the visualizer
    enum class State { waitingForTrigger, collecting } state { State::waitingForTrigger };
};

//The actual UI component of the oscilloscope
template <typename SampleType>
class ScopeComponent : public juce::Component, private Timer 
{
public:
    //initialize the scope
    ScopeComponent(AudioBufferQueue<SampleType>& queueToUse)
    : audioBufferQueue(queueToUse) 
    {
        buffer.fill(SampleType(0)); //initialize
        setFramesPerSecond(30); //fps
    }

    //setup timer
    void setFramesPerSecond(int framesPerSecond) 
    {
        jassert(framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz(framesPerSecond); //timer function called from inheritance
    }

    //draw the waveforms for oscillator and audio frequency spectrum
    void paint(Graphics& g) override 
    {
        g.setColour(juce::Colours::seashell); //coloring the waveforms
        auto area = getLocalBounds();
        auto h = (SampleType)area.getHeight();
        auto w = (SampleType)area.getWidth();
        // Oscilloscope
        auto scopeRect = juce::Rectangle<SampleType>{ SampleType(0), SampleType(0), w, h / 2 };
        plot(buffer.data(), buffer.size(), g, scopeRect, SampleType(1), h / 4);
        // Audio frequency spectrum
        auto spectrumRect = juce::Rectangle<SampleType>{ SampleType(0), h / 2, w, h / 2 };
        plot(spectrumData.data(), spectrumData.size() / 4, g, spectrumRect);
    }

    //no need to resize anything. pure virtual has to be implemented..
    void resized() override {}

private:
    AudioBufferQueue<SampleType>& audioBufferQueue; //holds audioBuffers
    std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
    juce::dsp::FFT fft{ AudioBufferQueue<SampleType>::order }; //fast fourier transform
    juce::dsp::WindowingFunction<SampleType> windowFun{ (size_t)fft.getSize(),
                juce::dsp::WindowingFunction<SampleType>::hann }; //juce thanks for the windowing function
    std::array<SampleType, 2 * AudioBufferQueue<SampleType>::bufferSize> spectrumData;

    //clear buffer and draw the wave sample and frequency spectrum
    void timerCallback() override 
    {
        audioBufferQueue.pop(buffer.data());
        FloatVectorOperations::copy(spectrumData.data(), buffer.data(), (int)buffer.size());
        jassert(spectrumData.size() == 2 * (size_t)fft.getSize());
        windowFun.multiplyWithWindowingTable(spectrumData.data(), (size_t)fft.getSize());
        fft.performFrequencyOnlyForwardTransform(spectrumData.data());
        static constexpr auto mindB = SampleType(-160); //bounds for
        static constexpr auto maxdB = SampleType(0); //decibels
        for (auto& s : spectrumData) //for each piece of frequency spectrum data
            s = jmap(jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(s) - juce::Decibels::gainToDecibels(SampleType((size_t)fft.getSize()))), mindB, maxdB, SampleType(0), SampleType(1));
        repaint(); //recalling paint and replot
    }

    //draw the actual waves using jmap and data
    static void plot(const SampleType* data, size_t numSamples, Graphics& g, juce::Rectangle<SampleType> rect,
    SampleType scale = SampleType(0.7), SampleType offset = SampleType(0)) 
    {
        auto w = rect.getWidth();
        auto h = rect.getHeight();
        auto right = rect.getRight();
        auto center = rect.getBottom() - offset;
        auto gain = h * scale;
        for (size_t i = 1; i < numSamples; ++i) //draw every piece of the sample
	        g.drawLine({ jmap(SampleType(i - 1), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), 
	        SampleType(right)), center - gain * data[i - 1], jmap(SampleType(i), SampleType(0), 
	        SampleType(numSamples - 1), SampleType(right - w), SampleType(right)), center - gain * data[i] });
    }
};
