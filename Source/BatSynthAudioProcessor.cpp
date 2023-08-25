#include "BatSynthAudioProcessor.h"
#include "BatSynthAudioProcessorEditor.h"

const int NUM_VOICES = 8;

BatSynthAudioProcessor::BatSynthAudioProcessor()
:   BaseAudioProcessor{ createParameterLayout() }
{
    addAllSynthControls();
}

void BatSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    BatSynthSettings::sampleRate = static_cast<int>(sampleRate);
    mySynth.setCurrentPlaybackSampleRate(sampleRate);
    midiCollector.reset(sampleRate);
}


void BatSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float lowestSynthFrequency = 4186.01f;
    for (int i = 0; i < mySynth.getNumVoices(); ++i) 
    {
        myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i));
        if (myVoice != nullptr) 
        {
            myVoice->setAttack(apvts.getRawParameterValue("attack"));
            myVoice->setDecay(apvts.getRawParameterValue("decay"));
            myVoice->setSustain(apvts.getRawParameterValue("sustain"));
            myVoice->setRelease(apvts.getRawParameterValue("release"));
            myVoice->setOsc2Pitch(apvts.getRawParameterValue("osc2Pitch"));
            myVoice->setOsc2Gain(apvts.getRawParameterValue("osc2Gain"));
            myVoice->setNoiseGain(apvts.getRawParameterValue("noiseGain"));
            myVoice->setFilterCutoff(apvts.getRawParameterValue("filterCutoff"));
            myVoice->setFilterResonance(apvts.getRawParameterValue("filterResonance"));
            myVoice->setLfoFilterIntensity(apvts.getRawParameterValue("lfoFilterIntensity"));
            myVoice->setLfoFilterRate(apvts.getRawParameterValue("lfoFilterRate"));
            myVoice->setLfoPitchIntensity(apvts.getRawParameterValue("lfoPitchIntensity"));
            myVoice->setLfoPitchRate(apvts.getRawParameterValue("lfoPitchRate"));
            myVoice->setTotalGain(apvts.getRawParameterValue("totalGain"));
            myVoice->setOsc1Type(apvts.getRawParameterValue("wavetype"));
            myVoice->setOsc2Type(apvts.getRawParameterValue("wavetype2"));
            if (myVoice->isVoiceActive() && myVoice->getFrequency() < lowestSynthFrequency)
                lowestSynthFrequency = static_cast<float>(myVoice->getFrequency());
        }
    }
    currentFrequency = lowestSynthFrequency;

    buffer.clear();
    midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples()); // [11]
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
  
    float rms = 0.0f;
    int numOutputs = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();
    for (int i = 0; i < numOutputs; ++i)
    {
        auto channelData = buffer.getReadPointer(i);
        rms += std::inner_product(channelData, channelData + numSamples, channelData, 0.0f);
    }
    rms /= buffer.getNumSamples() * buffer.getNumChannels();
    rms = std::sqrt(rms);
  
    currentAmplitude = rms;
  
    waveScopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples(), currentFrequency);
    spectrumScopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples(), currentFrequency);
}

AudioProcessorEditor* BatSynthAudioProcessor::createEditor()
{
  return new BatSynthAudioProcessorEditor (*this);
}

void BatSynthAudioProcessor::addAllSynthControls()
{
    mySynth.clearVoices(); 
    for (int i = 0; i < NUM_VOICES; ++i) 
        mySynth.addVoice(new SynthVoice());
    mySynth.clearSounds();
    mySynth.addSound(new SynthSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout BatSynthAudioProcessor::createParameterLayout()
{
    NormalisableRange<float> attackRange(0.1f, 5000.0f), decayRange(0.1f, 2000.0f), sustainRange(0.0f, 1.0f),
        releaseRange(0.1f, 5000.0f), wavetypeRange(0, 12), wavetype2Range(0, 12),
        osc2GainRange(0.f, 1.f), noiseGainRange(0.f, 1.f), osc2PitchRange(0.5f, 2.f),
        filterCutRange(50.f, 7100.f), filterResonRange(1.f, 10.f), lfoFilterIntenRange(0.f, 0.9f),
        lfoFilterRateRange(0.5f, 12.0f), lfoPitchIntenRange(0.0f, 0.9f), lfoPitchRateRange(0.05f, 6.0f), totalGainRange(0.f, 1.0f);
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    using Parameter = AudioProcessorValueTreeState::Parameter; //shortens the next few lines a bit
    layout.add(std::make_unique<Parameter>("attack", "Attack", "Attack", attackRange, 0.1f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("decay", "Decay", "Decay", decayRange, 500.f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("sustain", "Sustain", "Sustain", sustainRange, 0.8f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("release", "Release", "Release", releaseRange, 0.1f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("osc2Pitch", "Osc 2 Pitch", "Osc 2 Pitch", osc2PitchRange, 1.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("osc2Gain", "Osc 2 Gain", "Osc 2 Gain", osc2GainRange, 0.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("noiseGain", "Noise Gain", "Noise Gain", noiseGainRange, 0.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("filterCutoff", "Filter Cutoff", "Filter Cutoff", filterCutRange, 500.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("filterResonance", "Filter Resonance", "Filter Resonance", filterResonRange, 1.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("lfoFilterIntensity", "lfo Filter Intensity", "lfo Filter Intensity", lfoFilterIntenRange, 0.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("lfoFilterRate", "lfo Filter Rate", "lfo Filter Rate", lfoFilterRateRange, 2.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("lfoPitchIntensity", "lfo Pitch Intensity", "lfo Pitch Intensity", lfoPitchIntenRange, 0.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("lfoPitchRate", "lfo Pitch Rate", "lfo Pitch Rate", lfoPitchRateRange, 0.05f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("totalGain", "Total Gain", "Total Gain", totalGainRange, 1.0f, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("wavetype", "Wavetype", "Wavetype", wavetypeRange, 1, nullptr, nullptr));
    layout.add(std::make_unique<Parameter>("wavetype2", "Wavetype 2", "Wavetype 2", wavetype2Range, 0, nullptr, nullptr));
    return layout;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() 
{
    return new BatSynthAudioProcessor();
}
