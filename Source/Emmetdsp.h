#pragma once
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <vector>

#define PI  3.1415926535897932384626433832795
#define TWOPI 6.283185307179586476925286766559

class emmetSettings 
{
public:
	static int sampleRate;
};

class emmetOsc 
{
	double frequency;
	double phase;
	double output;
	double tri;
public:
	emmetOsc();
	double sinewave(double frequency);
	double coswave(double frequency);
	double saw(double frequency);
	double sawr(double frequency);
	double sawe(double frequency);
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
};

class emmetDelayLine {
public:
    static const int MAX_DELAY = 88200;
    std::vector<float> buffer;
    int writePointer = 0;
    float time = 0.0;
    float feedback = 0.0;

    emmetDelayLine() { buffer.resize(MAX_DELAY, 0.0); }
    void setDelayTime(float delayTime) { time = delayTime; }
    void setFeedback(float fdbk) { feedback = fdbk; }
    float process(float input);

private:
    void write(float input);
    float read(float delayTime);
};


class emmetFilter 
{
	double gain { 0.0 };
	double input { 0.0 };
	double output { 0.0 };
	double inputs[10] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double outputs[10] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double cutoff1 { 0.0 };
	double x { 0.0 }; //speed
	double y { 0.0 }; //pos
	double z { 0.0 }; //pole
	double c { 0.0 }; //filter coefficient
public:
	emmetFilter() {};
	double cutoff { 500.0 };
	double resonance { 1.0 };
	double lores(double input,double cutoff1, double resonance);
	double hires(double input,double cutoff1, double resonance);
	double bandpass(double input, double cutoff1, double resonance);
	double lopass(double input,double cutoff);
	double hipass(double input,double cutoff);
};

class emmetLPFilter {
private:
    double a;
    double prevOutput;

public:
    emmetLPFilter() : prevOutput(0.0) {}
    void setCutoffFrequency(double cutoff) { a = exp(-TWOPI * cutoff); }
    double process(double input) { prevOutput = (1 - a) * input + a * prevOutput; return prevOutput; }
};

class emmetCompressor
{
public:
    double ratio;
    double threshold;
    double attackTime;
    double releaseTime;
    double envelope;
    double gain;

    emmetCompressor() : ratio(1.0), threshold(0.0), attackTime(0.1), releaseTime(0.1), envelope(0.0), gain(1.0) {}
    emmetCompressor(double r, double t, double atk, double rel) : ratio(r), threshold(t), attackTime(atk), releaseTime(rel),
                                                             envelope(0.0), gain(1.0) {}
    double process(double input);
};

class emmetEnv 
{
public:
	double ar(double input, double attack=1, double release=0.9, long holdtime=1, int trigger=0);
	double adsr(double input, double attack=1, double decay=0.99, double sustain=0.125, double release=0.9, long holdtime=1, int trigger=0);
	double adsr(double input,int trigger);
	double input;
	double output;
	double attack;
	double decay;
	double sustain;
	double release;
	double amplitude;
	void setAttack(double attackMS);
	void setRelease(double releaseMS);
	void setDecay(double decayMS);
	void setSustain(double sustainL);
	int trigger = 0;
	long holdtime = 1;
	long holdcount;
	int attackphase,decayphase,sustainphase,holdphase,releasephase;
};

class emmetDistortion 
{
public:
	double fastatan(const double input);
	double tanhDist(const double input, const double shape);
	double atanDist(const double input, const double shape);
	double fastAtanDist(const double input, const double shape);
	double sigmoidDist(const double input, const double shape);
	double squareDist(const double, const double);
};

inline double emmetDistortion::fastatan(double x) 
{
	return (x / (1.0 + 0.28 * (x * x))); //
}

inline double emmetDistortion::tanhDist(double input, const double shape) 
{
	return (1.0 / tanh(shape)) * tanh(input * (shape/2))/ 1.8;
}

inline double emmetDistortion::atanDist(double input, const double shape) 
{
	return (1.0 / atan(shape)) * atan(input * (shape/2)) /1.8;
}

inline double emmetDistortion::fastAtanDist(double input, const double shape) 
{
	return (1.0 / fastatan(shape)) * fastatan(input * (shape/2)) / 2.4;
}

inline double emmetDistortion::sigmoidDist(double input, const double shape) 
{
	return cbrt(input*(shape/4.))/3.;
}

inline double emmetDistortion::squareDist(double input, double shape) 
{
	return sqrt(input*shape);
}
