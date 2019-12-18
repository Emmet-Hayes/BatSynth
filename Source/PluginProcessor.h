/*
  ==============================================================================

 PluginProcessor.h
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes
 
 The audio processor implements the functions that are required to run both audio
 and MIDI messages. it will search for and accept any MIDI it may be able to find,
 using multiple JUCE preprocessor directives to interface with the connected
 hardware efficiently. It also holds mutator values in a value tree that can be
 used to pass new values from the user interface to the audio thread! the processor
 also passes audio to the oscilloscope data collector for visualisation.
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSounds.h"
#include "SynthVoice.h"
#include "ScopeComponent.h"

class SynthFrameworkAudioProcessor  : public AudioProcessor {
public:
  //==============================================================================
  SynthFrameworkAudioProcessor();
  //SynthFrameworkAudioProcessor(MidiKeyboardState& keyState);
  ~SynthFrameworkAudioProcessor();

  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;


  #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
  #endif

  void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const String getProgramName (int index) override;
  void changeProgramName (int index, const String& newName) override;

  //==============================================================================
  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  void setAttackTime(const float newAtkTime) { attackTime = newAtkTime; }
  void setDecayTime(const float newDcyTime) { decayTime = newDcyTime; }
  void setSustainTime(const float newSusTime) { sustainTime = newSusTime; }
  void setReleaseTime(const float newRelTime) { releaseTime = newRelTime; }
  void setOsc2Gain(const float newGain) { osc2Gain = newGain; }
  void setNoiseGain(const float newGain) { noiseGain = newGain; }
  void setOsc2Pitch(const float newPitch) { osc2Pitch = newPitch; }
  void setFilterCutoff(const float newFiltCut) { filterCutoff = newFiltCut; }
  void setFilterResonance(const float newFiltRes) { filterResonance = newFiltRes; }
  void setTotalGain(const float totGain) { totalGain = totGain; }
  void setCompressionRatio(const float newRatio) { compressionRatio = newRatio; }
  void setCompressionThreshold(const float newThreshold) { compressionThreshold = newThreshold; }
  void setCompressionAttack(const float newAttack) { compressionAttack = newAttack; }
  void setCompressionRelease(const float newRelease) { compressionRelease = newRelease; }
  void setCompressionGain(const float newGain) { compressionGain = newGain; }
  void setDistortionDrive(const float newDrive) { distortionDrive = newDrive; }
  void setDelayTime(const float newTime) { delayTime = newTime; }
  void setDelayFeedback(const float newFeedback) { delayFeedback = newFeedback; }
  void setDelayGain(const float newGain) { delayGain = newGain; }
  void setLfoFilterIntensity(const float newGain) { lfoFilterIntensity = newGain; }
  void setLfoFilterRate(const float newRate) { lfoFilterRate = newRate; }
  /*void setLfoPitchIntensity(const float newGain) { lfoPitchIntensity = newGain; }
  void setLfoPitchRate(const float newRate) { lfoPitchRate = newRate; }*/
  void setOsc1Type(const juce::String newType) { osc1Type = newType; }
  void setOsc2Type(const juce::String newType) { osc2Type = newType; }
  void setDistType(const juce::String newType) { distType = newType; }
  AudioBufferQueue<float> audioBufferQueue; //used for oscilloscope
  //MidiKeyboardState keyboardState;
  //MidiMessageCollector midiCollector;
  AudioProcessorValueTreeState tree; //link values between sliders, comboBoxes and processor
private:
  Synthesiser mySynth;
  SynthVoice* myVoice;

  float attackTime, decayTime, sustainTime, releaseTime, osc2Gain, noiseGain, osc2Pitch,
    filterCutoff, filterResonance, lfoFilterIntensity, lfoFilterRate, /*lfoPitchIntensity, lfoPitchRate, */
    compressionRatio, compressionThreshold, compressionAttack, compressionRelease,
    distortionDrive, compressionGain, delayTime, delayFeedback, delayGain, totalGain;
  juce::String osc1Type, osc2Type, distType;
  ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue }; //oscilloscope!!

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthFrameworkAudioProcessor)
};
