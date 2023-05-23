/*
  ==============================================================================

    SynthVoice.h
    Created: 3 Sep 2019 11:46:10pm
    Author:  Emmet Hayes

 
 SynthVoice is responsible for the actual creation of sound through DSP. It calls
 the DSP functions in Emmetdsp.h in renderNextBlock to generate all the audio for each voice.
 any change in value on the hardware level is also detected here, including the pitch and mod wheel,
 and also other various controls commonly found on MIDI keyboards. There are 3 virtual functions from its
 parent class SynthesiserVoice that I had to implement, canPlaySound, startNote, and stopNote are all
 overridden member functions that I had to implement. there are also accessors and mutators for each
 variable parameter in the DSP signal chain.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSounds.h"
#include "Emmetdsp.h"

class SynthVoice : public SynthesiserVoice {
public:
    //pure virtual override function: returns true if a sound can be played
    bool canPlaySound(SynthesiserSound* sound) override 
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr; //if the sound can be cast as derived class it can play sound
    }

    //pure virtual override function: starts the note taking the note number, velocity, the sound and the current pitch position
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override 
    {
        env1.trigger = 1; //whether the envelope is being triggered 1 is true
        level = velocity;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }

    //stops the note taking the velocity and tailoff of the note
    void stopNote(float velocity, bool allowTailOff) override 
    {
        env1.trigger = 0;
        allowTailOff = true;
        if (velocity == 0)
            clearCurrentNote();
    }

    //setters and getters for all of the DSP-side variable values.
    //these all correspond to the user interface boxes, knobs and sliders.
    void setAttack(std::atomic<float> * attack) { env1.setAttack((double)* attack); }
    void setDecay(std::atomic<float> * decay) { env1.setDecay((double)* decay); }
    void setSustain(std::atomic<float> * sustain) { env1.setSustain((double)* sustain); }
    void setRelease(std::atomic<float> * release) { env1.setRelease((double)* release); }
    void setTotalGain(std::atomic<float> * lvl) { totalGain = (double)*lvl; }
    double getTotalGain() const { return totalGain; }
    void setFilterCutoff(std::atomic<float> * filtCut) { filt1.cutoff = (double)*filtCut; }
    double getFilterCutoff() const { return filt1.cutoff; }
    void setFilterResonance(std::atomic<float> * filtRes) { filt1.resonance = (double)*filtRes; }
    double getFilterResonance() const { return filt1.resonance; }
    void setCompressionRatio(std::atomic<float> * ratio) { comp1.setRatio((double)*ratio); compressGain = -0.05 * (double)*ratio + 0.9; }
    double getCompressionRatio() const { return comp1.ratio; }
    void setCompressionThreshold(std::atomic<float> * thresh) { comp1.setThreshold((double)*thresh); }
    double getCompressionThreshold() const { return comp1.threshold; }
    void setCompressionAttack(std::atomic<float> * attack) { comp1.setAttack((double)*attack); }
    double getCompressionAttack() const { return comp1.attack; }
    void setCompressionRelease(std::atomic<float> * release) { comp1.setRelease((double)*release); }
    double getCompressionRelease() const { return comp1.release; }
    void setCompressionGain(std::atomic<float> * gain) { compressGain = (double)*gain; }
    double getCompressionGain() const { return compressGain; }
    void setDelayTime(std::atomic<float> * time) { delay1.time = (double)*time; }
    double getDelayTime() const { return delay1.time; }
    void setDelayFeedback(std::atomic<float> * feedback) { delay1.feedback = (double)*feedback; }
    double getDelayFeedback() const { return delay1.feedback; }
    void setDelayGain(std::atomic<float> * gain) { delayGain = (double)*gain; }
    double getDelayGain() const { return delayGain; }
    void setOsc1Type(std::atomic<float> * oscType) { theWave1 = (double)*oscType + 0.1f; } //chosen type doesn't get rounded down
    double getOsc1Type() 
    {
        switch (theWave1) 
        { //picks the wave type and also sets the frequency using all the possible variables
            case 0: default: return osc1.sinewave(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 1: return osc1.saw(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 2: return osc1.square(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 3: return osc1.softdistsine(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 4: return osc1.harddistsine(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 5: return osc1.triangle(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 6: return osc1.noise();
            case 7: return osc1.sawwane(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 8: return osc1.sawwax(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 9: return osc1.pulse(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.2);
            case 10: return osc1.pulse(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.3);
            case 11: return osc1.sawpulse(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.2);
            case 12: return osc1.sawpulse(frequency * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.3);
        }
    }
    void setOsc2Type(std::atomic<float> * oscType) { theWave2 = (double)*oscType + 0.1f; } //chosen type doesn't get rounded down
    double getOsc2Type() 
    {
        switch (theWave2) 
        { //see getOsc1Type() directly above
            case 0: default: return osc2.sinewave(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 1: return osc2.saw(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 2: return osc2.square(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 3: return osc2.softdistsine(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 4: return osc2.harddistsine(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 5: return osc2.triangle(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 6: return osc2.noise();
            case 7: return osc2.sawwane(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 8: return osc2.sawwax(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.));
            case 9: return osc2.pulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.2);
            case 10: return osc2.pulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.3);
            case 11: return osc2.sawpulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.2);
            case 12: return osc2.sawpulse(frequency * osc2PitchBend * oscAllPitchBend + lfoPitchIntensity * lfo2.sinewave(lfoPitchFreq) * (frequency/220.), 0.3);
        }
    }
    void setOsc2Gain(std::atomic<float> * newGain) { osc2Gain = (double)*newGain; }
    double getOsc2Gain() const { return osc2Gain; }
    void setNoiseGain(std::atomic<float> * newGain) { noiseGain = (double)*newGain; }
    double getNoiseGain() const { return noiseGain; }
    void setOsc2Pitch(std::atomic<float> * newPitch) { osc2PitchBend = (double)*newPitch; }
    double getOsc2Pitch() const { return osc2PitchBend; }
    void setLfoFilterIntensity(std::atomic<float> * newGain) { lfoFilterIntensity = (double)*newGain; }
    double getLfoFilterIntensity() const { return lfoFilterIntensity; }
    void setLfoFilterRate(std::atomic<float> * newRate) { lfoFilterFreq = (double)*newRate; }
    double getLfoFilterRate() const { return lfoFilterFreq; }
    void setDistDrive(std::atomic<float> * newDrive) { distDrive1 = (double)*newDrive; }
    double getDistDrive() const { return distDrive1; }
    void setDistType(std::atomic<float> * newType) { distChoice1 = (double)*newType + 0.1f; }
    double getDistSignal(const double signal) 
    {
        switch (distChoice1) 
        {
            case 0: default: return signal;
            case 1: return dist1.sigmoidDist(signal, distDrive1);
            case 2: return dist1.tanhDist(signal, distDrive1);
            case 3: return dist1.atanDist(signal, distDrive1);
            case 4: return dist1.fastAtanDist(signal, distDrive1);
        }
    }
    /*void setLfoPitchIntensity(const float *newGain) { lfoPitchIntensity = *newGain; }
    double getLfoPitchIntensity() const { return lfoPitchIntensity; }
    void setLfoPitchRate(const float * newRate) { lfoPitchFreq = *newRate; }
    double getLfoPitchRate() const { return lfoPitchFreq; }*/
  
    //if the hardware is a keyboard with a pitch wheel, it will bend the note an octave.
    void pitchWheelMoved(int newPitchWheelValue) override 
    {
        oscAllPitchBend = (newPitchWheelValue < 8192.) ? 0.5 + (newPitchWheelValue / 16384.) :
                                                         2.0 * (newPitchWheelValue / 16384.);
    }

    //if the extra controls are moved, react accordingly to standard MIDI message numbers.
    void controllerMoved(int controllerNumber, int newControllerValue) override 
    {
        if (controllerNumber == 1) lfoPitchIntensity = (newControllerValue / 127.) * 4.0;
        if (controllerNumber == 7) keyboardVolume = newControllerValue / 127.;
        if (controllerNumber == 10) lfoPitchFreq = 0.5 + (4.5 * (newControllerValue / 127.));
    }

    //calling all DSP functions here in blocks...
    /* Signal flow diagram
    [Osc1]-----------------\,
    [Osc2]*gain*pitch-----++=>-*level-->-[ADSR]--->-[LORES]->-[COMPRESS]-->-[DELAY]-->-[LIMITER]
    [Noise]*gain-----------/'
    */
    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override 
    {
        for (int sample = 0; sample < numSamples; ++sample) 
        {
            double theSound = (getOsc1Type() + (getOsc2Type() * osc2Gain) + (noise1.noise() * noiseGain))/ 3.; //combines 3 oscs
            theSound *= level * keyboardVolume; //controlling the level here with
            theSound = env1.adsr(theSound, env1.trigger); //adsr envelope called with trigger activated
            theSound = getDistSignal(theSound);
            theSound = filt1.lores(theSound, getFilterCutoff() + lfoFilterIntensity
            * lfo1.sinewave(lfoFilterFreq), getFilterResonance()); //lo 12db/oct resonance filter with lfo filter intensity and rate included
            theSound =  compressGain * comp1.compressor(theSound, getCompressionRatio(), getCompressionThreshold(),
            getCompressionAttack(), getCompressionRelease()); //compressor with 5 params
            theSound += delay1.dl(theSound, getDelayTime(), getDelayFeedback()) * delayGain; //delay with 3 params
            theSound *= getTotalGain(); //final gain
            theSound = limit1.limiter(theSound); //final limiter
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) //for each channel of audio
            outputBuffer.addSample(channel, startSample, theSound); //add each sample to the buffer
            ++startSample; //tick per sample buffer added, pre-increment helps alot here
        }
    }

private:
    double level, frequency, totalGain = 1.0, delayGain = 0.7, compressGain = 0.8, noiseGain = 0.0,
    osc2Gain = 0.0, osc2PitchBend = 1.0, lfoFilterFreq = 1.0, lfoFilterIntensity = 0.0,
    oscAllPitchBend = 1.0, keyboardVolume = 1.0, lfoPitchFreq = 2.0, lfoPitchIntensity = 0.0,
    distDrive1 = 1.0;
    int theWave1, theWave2, distChoice1; 
    emmetOsc osc1, osc2, noise1, lfo1, lfo2; // 3 oscillators for sounds, 2 lfos - lfo1 is filter lfo2 is pitch
    emmetEnv env1; //adsr envelope
    emmetFilter filt1; //lo resonant filter
    emmetDistortion dist1; //optional distortion
    emmetDyn comp1, limit1; //compressor and final limiter
    emmetFractionalDelay delay1; //sample based delay circuit
};
