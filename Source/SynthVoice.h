#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSounds.h"
#include "BatSynthDSP.h"

class SynthVoice : public SynthesiserVoice 
{
public:
    bool canPlaySound(SynthesiserSound* sound) override 
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr; 
    }

    void startNote(int midiNoteNumber, float velocity,
        SynthesiserSound*, int) override 
    {
        env1.trigger = 1; 
        level = velocity;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }

    void stopNote(float velocity, bool allowTailOff) override 
    {
        env1.trigger = 0;
        allowTailOff = true;
        if (velocity == 0)
            clearCurrentNote();
    }

    void setAttack(const std::atomic<float>* attack) { env1.setAttack((double)* attack); }
    void setDecay(const std::atomic<float>* decay) { env1.setDecay((double)* decay); }
    void setSustain(const std::atomic<float>* sustain) { env1.setSustain((double)* sustain); }
    void setRelease(const std::atomic<float>* release) { env1.setRelease((double)* release); }
    void setTotalGain(const std::atomic<float>* lvl) { totalGain = (double)*lvl; }
    void setFilterCutoff(const std::atomic<float>* filtCut) { filt1.cutoff = (double)*filtCut; }
    void setFilterResonance(const std::atomic<float>* filtRes) { filt1.resonance = (double)*filtRes; }
    void setCompressionGain(const std::atomic<float>* gain) { compressGain = (double)*gain; }
    void setDelayGain(const std::atomic<float>* gain) { delayGain = (double)*gain; }
    void setOsc2Gain(const std::atomic<float>* newGain) { osc2Gain = (double)*newGain; }
    void setNoiseGain(const std::atomic<float>* newGain) { noiseGain = (double)*newGain; }
    void setOsc2Pitch(const std::atomic<float>* newPitch) { osc2PitchBend = (double)*newPitch; }
    void setLfoFilterIntensity(const std::atomic<float>* newGain) { lfoFilterIntensity = (double)*newGain; }
    void setLfoFilterRate(const std::atomic<float>* newRate) { lfoFilterFreq = (double)*newRate; }
    void setLfoPitchIntensity(const std::atomic<float>* newGain) { lfoPitchIntensity = *newGain; }
    void setLfoPitchRate(const std::atomic<float>* newRate) { lfoPitchFreq = *newRate; }
    void setDistDrive(const std::atomic<float>* newDrive) { distDrive1 = (double)*newDrive; }
    void setOsc1Type(const std::atomic<float>* oscType) { theWave1 = static_cast<int>((double)*oscType + 0.1); }
    void setOsc2Type(const std::atomic<float>* oscType) { theWave2 = static_cast<int>((double)*oscType + 0.1); }
    void setDistType(const std::atomic<float>* newType) { distChoice1 = static_cast<int>((double)*newType + 0.1); }
    
    double getFrequency() const { return frequency; }
    double getTotalGain() const { return totalGain; }
    double getFilterCutoff() const { return filt1.cutoff; }
    double getFilterResonance() const { return filt1.resonance; }
    double getCompressionGain() const { return compressGain; }
    double getDelayGain() const { return delayGain; }
    double getOsc2Gain() const { return osc2Gain; }
    double getNoiseGain() const { return noiseGain; }
    double getOsc2Pitch() const { return osc2PitchBend; }
    double getLfoFilterIntensity() const { return lfoFilterIntensity; }
    double getLfoFilterRate() const { return lfoFilterFreq; }
    double getLfoPitchIntensity() const { return lfoPitchIntensity; }
    double getLfoPitchRate() const { return lfoPitchFreq; }
    double getDistDrive() const { return distDrive1; }

    double getOsc1Type() 
    {
        switch (theWave1) 
        { 
            case 0: default: return osc1.sinewave(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 1: return osc1.saw(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 2: return osc1.square(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 3: return osc1.softdistsine(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 4: return osc1.harddistsine(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 5: return osc1.triangle(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 6: return osc1.noise();
            case 7: return osc1.sawwane(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 8: return osc1.sawwax(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 9: return osc1.pulse(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.2);
            case 10: return osc1.pulse(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.3);
            case 11: return osc1.sawpulse(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.2);
            case 12: return osc1.sawpulse(frequency * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.3);
        }
    }
    double getOsc2Type() 
    {
        switch (theWave2) 
        {
            case 0: default: return osc2.sinewave(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 1: return osc2.saw(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 2: return osc2.square(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 3: return osc2.softdistsine(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 4: return osc2.harddistsine(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 5: return osc2.triangle(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 6: return osc2.noise();
            case 7: return osc2.sawwane(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 8: return osc2.sawwax(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.));
            case 9: return osc2.pulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.2);
            case 10: return osc2.pulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.3);
            case 11: return osc2.sawpulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.2);
            case 12: return osc2.sawpulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * (lfo2.sinewave(lfoPitchFreq) * 5) * (frequency/220.), 0.3);
        }
    }
    
    void pitchWheelMoved(int newPitchWheelValue) override 
    {
        oscAllPitchBend = (newPitchWheelValue < 8192.) ? 0.5 + (newPitchWheelValue / 16384.) :
            2.0 * (newPitchWheelValue / 16384.);
    }

    void controllerMoved(int controllerNumber, int newControllerValue) override 
    {
        if (controllerNumber == 1) lfoPitchIntensity = (newControllerValue / 127.) * 5.0;
        if (controllerNumber == 7) keyboardVolume = newControllerValue / 127.;
        if (controllerNumber == 10) lfoPitchFreq = 0.5 + (4.5 * (newControllerValue / 127.));
    }

    /* Signal flow diagram
     [Osc1]-----------------\,
     [Osc2]*gain*pitch-----++=>-*level-->-[ADSR]--->-[LORES]-->-[LIMITER]
     [Noise]*gain-----------/'
     */
    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override 
    {   
        for (int sample = 0; sample < numSamples; ++sample) 
        {
            double theSound = (getOsc1Type() + (getOsc2Type() * osc2Gain) + (noise1.noise() * noiseGain))/ 3.;
            theSound *= level * keyboardVolume;
            theSound = env1.adsr(theSound, env1.trigger);
            theSound = filt1.lores(theSound, getFilterCutoff() + lfoFilterIntensity
                * lfo1.sinewave(lfoFilterFreq), getFilterResonance());
            theSound *= getTotalGain();
            theSound = limit1.process(theSound);
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                outputBuffer.addSample(channel, startSample, theSound);
            ++startSample;
        }
    }

private:
    double level, frequency, totalGain = 1.0, delayGain = 0.7, compressGain = 0.8, noiseGain = 0.0,
        osc2Gain = 0.0, osc2PitchBend = 1.0, lfoFilterFreq = 1.0, lfoFilterIntensity = 0.0,
        oscAllPitchBend = 1.0, keyboardVolume = 1.0, lfoPitchFreq = 2.0, lfoPitchIntensity = 0.0,
        distDrive1 = 1.0;
    int theWave1, theWave2, distChoice1;
    BatSynthOsc osc1, osc2, noise1, lfo1, lfo2;
    BatSynthEnv env1; 
    BatSynthFilter filt1;
    BatSynthCompressor limit1 { 20.0, 0.9, 10.0, 100.0 };
};
