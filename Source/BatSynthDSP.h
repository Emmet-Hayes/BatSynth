#pragma once

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <vector>

#define PI  3.1415926535897932384626433832795
#define TWOPI 6.283185307179586476925286766559


class BatSynthSettings 
{
public:
    static int sampleRate;
};


class BatSynthOsc 
{
public:
    double sinewave(double frequency);
    double coswave(double frequency);
    double saw(double frequency);
    double sawr(double frequency);
    double triangle(double frequency);
    double square(double frequency);
    double sawwane(double frequency);
    double sawwax(double frequency);
    double sawpulse(double frequency, double duty);
    double pulse(double frequency, double duty);
    double noise();
    double softdistsine(double frequency);
    double harddistsine(double frequency);
    void phaseReset(double phaseIn);

private:
    double phase = 0.0;
    double output;
};


class BatSynthFilter 
{
public:
    BatSynthFilter() {};
    double cutoff { 500.0 };
    double resonance { 1.0 };
    double lores(double input,double cutoff1, double resonance);
    double hires(double input,double cutoff1, double resonance);
    double bandpass(double input, double cutoff1, double resonance);
    double lopass(double input,double cutoff);
    double hipass(double input,double cutoff);

private:
    double gain        { 0.0 };
    double input       { 0.0 };
    double output      { 0.0 };
    double inputs[10]  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double outputs[10] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cutoff1     { 0.0 };
    double x { 0.0 }; // speed
    double y { 0.0 }; // pos
    double z { 0.0 }; // pole
    double c { 0.0 }; // filter coefficient
};

class BatSynthCompressor
{
public:
    double ratio      { 1.0 };
    double threshold  { 0.0 };
    double attackTime { 0.1 };
    double releaseTime{ 0.1 };
    double envelope   { 0.0 };
    double gain       { 1.0 };

    BatSynthCompressor(double r, double t, double atk, double rel) : ratio(r), threshold(t), attackTime(atk), releaseTime(rel) {}
    double process(double input);
};

class BatSynthLPFilter {
public:
    void setCutoffFrequency(double cutoff) { a = exp(-TWOPI * cutoff); }
    double process(double input) { prevOutput = (1 - a) * input + a * prevOutput; return prevOutput; }

private:
    double a          { 0.0 };
    double prevOutput { 0.0 };
};

class BatSynthEnv 
{
public:
    double adsr(double input,int trigger);
    void setAttack(double attackMS);
    void setRelease(double releaseMS);
    void setDecay(double decayMS);
    void setSustain(double sustainL);
    int trigger = 0;

private:
    double input, output, amplitude,
           attack, decay, sustain, release;
    long holdtime = 1;
    long holdcount;
    int attackphase, decayphase, sustainphase, holdphase, releasephase;
};