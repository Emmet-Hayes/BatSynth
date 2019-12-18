/*
  ==============================================================================

 PluginProcessor.cpp
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes

 The constructor fills the value tree with each parameter on the synthesizer
 initialized and within normalizable ranges. each value has a corresponding key
 which is simply its intended parameter name. processBlock encapsulates the entirety
 of the DSP, as well as the oscilloscope operations. in ProcessBlock, f
 rom within the value tree each parameter can be mutated, and is in fact always
 being mutated by the values being passed from the user interface.
 forget about everything other than fine dining and breathing right here.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//initializing the entirety of the mutatable value set that is the synthesizer's parameters.
SynthFrameworkAudioProcessor::SynthFrameworkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
      #if ! JucePlugin_IsSynth //preprocessor checking for inputs and outputs
        .withInput  ("Input",  AudioChannelSet::stereo(), true)
      #endif
      .withOutput ("Output", AudioChannelSet::stereo(), true)
    #endif
  ), tree(*this, nullptr)
#endif
{
  //keyboardState.reset();
  //These ranges will determine what our parameters are capaable of changing to. They should always line up
  //with the corresponding ranges set on the UI buttons and sliders themselves.
  NormalisableRange<float> attackRange(0.1f, 5000.0f), decayRange(0.1f, 2000.0f), sustainRange(0.0f, 1.0f),
    releaseRange(0.1f, 5000.0f), wavetypeRange(0, 12), wavetype2Range(0, 12),
    osc2GainRange(0.f, 1.f), noiseGainRange(0.f, 1.f), osc2PitchRange(0.5f, 2.f),
    filterCutRange(50.f, 7100.f), filterResonRange(1.f, 10.f), lfoFilterIntenRange(0.f, 0.9f),
    lfoFilterRateRange(0.5f, 12.0f), /*lfoPitchIntenRange0.0f, 3.0f), lfoPitchRateRange(0.05f, 4.0f), */
    compressRatioRange(1.f, 10.f),  compressGainRange(0.f, 1.f),
    compressThreshRange(0.1f, 1.f), compressAttackRange(0.5f, 100.f), compressReleaseRange(0.1f, 2.0f), distortionTypeRange(0, 4), distortionDriveRange(0.01f, 6.f),
    delayTimeRange(100.f, 88000.f), delayFeedbackRange(0.f, 0.9f), delayGainRange(0.f, 1.f), totalGainRange(0.f, 1.3f);
  using Parameter = AudioProcessorValueTreeState::Parameter; //shortens the next few lines a bit
  tree.createAndAddParameter(std::make_unique<Parameter>("attack", "Attack", "Attack", attackRange, 0.1f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("decay", "Decay", "Decay", decayRange, 500.f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("sustain", "Sustain", "Sustain", sustainRange, 0.8f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("release", "Release", "Release", releaseRange, 0.1f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("wavetype", "Wavetype", "Wavetype", wavetypeRange, 1, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("wavetype2", "Wavetype 2", "Wavetype 2", wavetype2Range, 0, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("osc2Gain", "Osc 2 Gain", "Osc 2 Gain", osc2GainRange, 0.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("noiseGain", "Noise Gain", "Noise Gain", noiseGainRange, 0.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("osc2Pitch", "Osc 2 Pitch", "Osc 2 Pitch", osc2PitchRange, 1.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("filterCutoff", "Filter Cutoff", "Filter Cutoff", filterCutRange, 500.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("filterResonance", "Filter Resonance", "Filter Resonance", filterResonRange, 1.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("lfoFilterIntensity", "lfo Filter Intensity", "lfo Filter Intensity", lfoFilterIntenRange, 0.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("lfoFilterRate", "lfo Filter Rate", "lfo Filter Rate", lfoFilterRateRange, 2.0f, nullptr, nullptr));
  /*tree.createAndAddParameter(std::make_unique<Parameter>("lfoPitchIntensity", "lfo Pitch Intensity", "lfo Pitch Intensity", lfoPitchIntenRange, 0.0f));
  tree.createAndAddParameter(std::make_unique<Parameter>("lfoPitchRate", "lfo Pitch Rate", "lfo Pitch Rate", lfoPitchRateRange, 0.05f));*/
  tree.createAndAddParameter(std::make_unique<Parameter>("compressionRatio", "Compression Ratio", "Compression Ratio", compressRatioRange, 2.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("compressionThreshold", "Compression Threshold", "Compression Threshold", compressThreshRange, 0.9f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("compressionAttack", "Compression Attack", "Compression Threshold", compressAttackRange, 1.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("compressionRelease", "Compression Release", "Compression Release", compressReleaseRange, 0.995f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("compressionGain", "Compression Gain", "Compression Gain", compressGainRange, 0.8f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("distortionType", "Distortion Type", "Distortion Type", distortionTypeRange, 0, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("distortionDrive", "Distortion Drive", "Distortion Drive", distortionDriveRange, 1.f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("delayTime", "Delay Time", "Delay Time", delayTimeRange, 10000.0f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("delayFeedback", "Delay Feedback", "Delay Feedback", delayFeedbackRange, 0.5f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("delayGain", "Delay Gain", "Delay Gain", delayGainRange, 0.5f, nullptr, nullptr));
  tree.createAndAddParameter(std::make_unique<Parameter>("totalGain", "Total Gain", "Total Gain", totalGainRange, 1.0f, nullptr, nullptr));
  tree.state = ValueTree("Empty"); //initialize tree state, otherwise it won't load
  mySynth.clearVoices(); //voices should be empty
  for (int i = 0; i < 8; ++i) //8 voices for up to 8 keys pressed at once. octophonic.
    mySynth.addVoice(new SynthVoice());
  mySynth.clearSounds(); //sounds should also be empty
  mySynth.addSound(new SynthSound()); //add the sound class items
  //midiCollector.reset(emmetSettings::sampleRate);
}

SynthFrameworkAudioProcessor::~SynthFrameworkAudioProcessor() {
}

//the following functions are JUCE default functions, they ask questions to our MIDI devices.
const String SynthFrameworkAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool SynthFrameworkAudioProcessor::acceptsMidi() const {
  #if JucePlugin_WantsMidiInput
    return true;
  #else
    return false;
  #endif
}

bool SynthFrameworkAudioProcessor::producesMidi() const {
  #if JucePlugin_ProducesMidiOutput
    return true;
  #else
    return false;
  #endif
}

bool SynthFrameworkAudioProcessor::isMidiEffect() const {
  #if JucePlugin_IsMidiEffect
    return true;
  #else
    return false;
  #endif
}

double SynthFrameworkAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SynthFrameworkAudioProcessor::getNumPrograms() {
  return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int SynthFrameworkAudioProcessor::getCurrentProgram()  {
  return 0;
}

void SynthFrameworkAudioProcessor::setCurrentProgram(int index)  {
}

const String SynthFrameworkAudioProcessor::getProgramName(int index) {
  return {};
}

void SynthFrameworkAudioProcessor::changeProgramName(int index, const String& newName) {

}

//set the sample rate of playback and ignore unused samples
void SynthFrameworkAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  ignoreUnused(samplesPerBlock);
  emmetSettings::sampleRate = sampleRate; //so that DSP knows the right sample rate
  mySynth.setCurrentPlaybackSampleRate(sampleRate); //so that JUCE knows the right sample rate lol
  //midiCollector.reset(sampleRate);
}

// When playback stops, you can use this as an opportunity to free up any spare memory, etc.
void SynthFrameworkAudioProcessor::releaseResources(){
}

//checks for type of output, mono/stereo, and also if input layout matches the output layout
#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthFrameworkAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
  //we only support mono or stereo for now. sorry film people and surround sound enthusiasts. :'(
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
    && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
  #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
      return false;
    #endif
    return true;
  #endif
}
#endif

//each block of audio is processed through this function, and this calls renderNextBlock for each voice in
//synthesizer. the values in the tree are passed to the DSP thread, while being constantly updated by the user interface
//thread. As long as the voice exists. Clear any previous audio in the buffer.
//the oscilloscope data is updated at last after the audio has already been rendered."
void SynthFrameworkAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
  for (int i = 0; i < mySynth.getNumVoices(); ++i) {
    myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i));
    if (myVoice != nullptr) { //each existing voice gets affected by every change in these tree values
      myVoice->setAttack(tree.getRawParameterValue("attack"));
      myVoice->setDecay(tree.getRawParameterValue("decay"));
      myVoice->setSustain(tree.getRawParameterValue("sustain"));
      myVoice->setRelease(tree.getRawParameterValue("release"));
      myVoice->setOsc1Type(tree.getRawParameterValue("wavetype"));
      myVoice->setOsc2Type(tree.getRawParameterValue("wavetype2"));
      myVoice->setOsc2Gain(tree.getRawParameterValue("osc2Gain"));
      myVoice->setNoiseGain(tree.getRawParameterValue("noiseGain"));
      myVoice->setOsc2Pitch(tree.getRawParameterValue("osc2Pitch"));
      myVoice->setFilterCutoff(tree.getRawParameterValue("filterCutoff"));
      myVoice->setFilterResonance(tree.getRawParameterValue("filterResonance"));
      myVoice->setLfoFilterIntensity(tree.getRawParameterValue("lfoFilterIntensity"));
      myVoice->setLfoFilterRate(tree.getRawParameterValue("lfoFilterRate"));
      /*myVoice->setLfoPitchIntensity(tree.getRawParameterValue("lfoPitchIntensity"));
       myVoice->setLfoPitchRate(tree.getRawParameterValue("lfoPitchRate")); */
      myVoice->setCompressionRatio(tree.getRawParameterValue("compressionRatio"));
      myVoice->setCompressionThreshold(tree.getRawParameterValue("compressionThreshold"));
      myVoice->setCompressionAttack(tree.getRawParameterValue("compressionAttack"));
      myVoice->setCompressionRelease(tree.getRawParameterValue("compressionRelease"));
      myVoice->setCompressionGain(tree.getRawParameterValue("compressionGain"));
      myVoice->setDistType(tree.getRawParameterValue("distortionType"));
      myVoice->setDistDrive(tree.getRawParameterValue("distortionDrive"));
      myVoice->setDelayTime(tree.getRawParameterValue("delayTime"));
      myVoice->setDelayFeedback(tree.getRawParameterValue("delayFeedback"));
      myVoice->setDelayGain(tree.getRawParameterValue("delayGain"));
      myVoice->setTotalGain(tree.getRawParameterValue("totalGain"));
    }
  }
  buffer.clear();
  //midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples()); // [11]
  //keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
  mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
  scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
}

bool SynthFrameworkAudioProcessor::hasEditor() const {
  return true; // so if anybody asks..
}

AudioProcessorEditor* SynthFrameworkAudioProcessor::createEditor() {
  return new SynthFrameworkAudioProcessorEditor (*this); //create the editor
}

//==============================================================================
void SynthFrameworkAudioProcessor::getStateInformation (MemoryBlock& destData) {
  std::unique_ptr<XmlElement> xml(tree.state.createXml());
  copyXmlToBinary(*xml, destData);
}

void SynthFrameworkAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
  std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(tree.state.getType()))
      tree.replaceState(ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new SynthFrameworkAudioProcessor();
}