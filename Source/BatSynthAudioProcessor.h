#pragma once

#include "../../Common/BaseAudioProcessor.h"
#include "../../Common/SpectrumScopeComponent.h"
#include "../../Common/WaveScopeComponent.h"
#include "../../Common/WaveTableComponent.h"

#include "BatSynth.h"
#include "BatSynthSounds.h"
#include "BatSynthVoice.h"


constexpr int NUM_SLIDERS = 14;
constexpr int NUM_COMBOBOXES = 2;

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

    void setCustomWaveform(const std::vector<float>& wave);

    AudioBufferQueue<float> audioBufferQueue;
    WaveScopeDataCollector<float> waveScopeDataCollector { audioBufferQueue };
    SpectrumScopeDataCollector<float> spectrumScopeDataCollector { audioBufferQueue };
    MidiKeyboardState keyboardState;
    MidiMessageCollector midiCollector;

private:
    void addAllSynthControls();

    BatSynth mySynth;
    BatSynthVoice* myVoice;

    float currentAmplitude { 0.0f };
    float currentFrequency { 4186.01f };

    static constexpr int NUM_VOICES{ 8 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BatSynthAudioProcessor)
};
