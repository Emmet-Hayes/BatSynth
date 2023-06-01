#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSounds.h"
#include "SynthVoice.h"
#include "ScopeComponent.h"

const int NUM_SLIDERS = 23;
const int NUM_COMBOBOXES = 3;

class SynthFrameworkAudioProcessor  : public AudioProcessor 
{
public:
	SynthFrameworkAudioProcessor();
	~SynthFrameworkAudioProcessor();

	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	#ifndef JucePlugin_PreferredChannelConfigurations
		bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
	#endif

	void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram (int index) override;
	const String getProgramName (int index) override;
	void changeProgramName (int index, const String& newName) override;
	void getStateInformation (MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;
	
	float getCurrentAmplitude() const { return currentAmplitude; }
	float getCurrentFrequency() const { return currentFrequency; }

	AudioBufferQueue<float> audioBufferQueue;
	ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue };
	//MidiKeyboardState keyboardState;
	//MidiMessageCollector midiCollector;
	AudioProcessorValueTreeState tree; //link values between sliders, comboBoxes and processor
private:
	void addAllControls();

	Synthesiser mySynth;
	SynthVoice* myVoice;
	float currentAmplitude = 0.0f;
	float currentFrequency = 4186.01f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrameworkAudioProcessor)
};
