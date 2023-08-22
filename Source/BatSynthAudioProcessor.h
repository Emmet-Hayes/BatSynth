#pragma once
#include <JuceHeader.h>
#include "SynthSounds.h"
#include "SynthVoice.h"
#include "../../Common/BaseAudioProcessor.h"
#include "../../Common/WaveScopeComponent.h"
#include "../../Common/SpectrumScopeComponent.h"

const int NUM_SLIDERS = 14;
const int NUM_COMBOBOXES = 2;

class BatSynthAudioProcessor  : public BaseAudioProcessor
{
public:
    BatSynthAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() override;


    float getCurrentAmplitude() const { return currentAmplitude; }
    float getCurrentFrequency() const { return currentFrequency; }

    AudioBufferQueue<float> audioBufferQueue;
    WaveScopeDataCollector<float> waveScopeDataCollector { audioBufferQueue };
    SpectrumScopeDataCollector<float> spectrumScopeDataCollector { audioBufferQueue };
    //MidiKeyboardState keyboardState;
    //MidiMessageCollector midiCollector;

private:
    void addAllControls();

    Synthesiser mySynth;
    SynthVoice* myVoice;
    float currentAmplitude = 0.0f;
    float currentFrequency = 4186.01f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BatSynthAudioProcessor)
};
