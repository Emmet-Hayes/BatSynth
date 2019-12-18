/*
 *  emmetdsp.h
 *  platform independent synthesis library using portaudio or rtaudio
 *
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 *  Created by Mick Grierson on 12/29/2009.
 *  Copyright 2009 Mick Grierson & Strangeloop Limited. All rights reserved.
 *  Edited and repurposed by Emmet Hayes on 9/1/2019.
 *  Thanks to the Goldsmiths Creative Computing Team.
 *  Special thanks to Arturo Castro for the PortAudio implementation.

 **************************************************************
 
 You will find all DSP declarations in this document.
 
 Class list:
 emmetSettings - master settings like sample rate, number of channels, buffer size
 emmetOsc - provides mathematical functions to produce sound based on frequency
 emmetDelayLine - helps chorus and flanger by providing a short line
 emmetFractionalDelay - a fully functioning digital delay module
 emmetFilter - sets of filtering functions to shape the harmonics of the waveform
 emmetDyn - compressor and gate functions to process dsp (frequency based)
 emmetEnv - attack decay sustain and release functions
 emmetDistortion - excite harmonics in the waveform based on frequency
 emmetFlanger - i think you get the picture..
 emmetChorus - short delay
 emmetSVF - still working on this.
 
 2046 lines of code in this whole project, pretty light considering what it can dos.
*/
#ifndef EMMETDSP_H
#define EMMETDSP_H

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#ifndef PI
#define PI  3.1415926535897932384626433832795
#endif
#define TWOPI 6.283185307179586476925286766559

class emmetSettings {
public:
  static int sampleRate;
};

class emmetOsc {
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
class emmetFractionalDelay {
  static const int delaySize = 88200;
  double memory[delaySize];
  int writePointer = 0, readPointer = 0;
public:
  emmetFractionalDelay ( void );
  double time = 10000.0, feedback = 0.5;
  double dl ( double sig, double delayTime, double feedback );
};

class emmetFilter {
  double gain;
  double input;
  double output;
  double inputs[10];
  double outputs[10];
  double cutoff1;
  double x; //speed
  double y; //pos
  double z; //pole
  double c; //filter coefficient
public:
  emmetFilter():x(0.0), y(0.0), z(0.0), c(0.0){};
  double cutoff;
  double resonance;
  double lores(double input,double cutoff1, double resonance);
  double hires(double input,double cutoff1, double resonance);
  double bandpass(double input,double cutoff1, double resonance);
  double lopass(double input,double cutoff);
  double hipass(double input,double cutoff);
};

/* OK this compressor and gate are now ready to use. The envelopes, like all the envelopes in this recent update, use stupid algorithms for
 incrementing - consequently a long attack is something like 0.0001 and a long release is like 0.9999.
 Annoyingly, a short attack is 0.1, and a short release is 0.99. I'll sort this out laters */
class emmetDyn {
public:
  double gate(double input, double threshold=0.9, long holdtime=1, double attack=1, double release=0.01);
  double compressor(double input, double ratio, double threshold=0.9, double attack=1, double release=0.01);
  double limiter(double unlimited);
  double input;
  double ratio;
  double currentRatio;
  double threshold;
  double output;
  double attack;
  double release;
  double amplitude;
  void setAttack(double attackMS);
  void setRelease(double releaseMS);
  void setThreshold(double thresholdI);
  void setRatio(double ratioF);
  long holdtime;
  long holdcount;
  int attackphase,holdphase,releasephase;
};

class emmetEnv {
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
  int trigger;
  long holdtime=1;
  long holdcount;
  int attackphase,decayphase,sustainphase,holdphase,releasephase;
};

class emmetDistortion {
public:
  double fastatan(const double input);
  double tanhDist(const double input, const double shape);
  double atanDist(const double input, const double shape);
  double fastAtanDist(const double input, const double shape);
  double sigmoidDist(const double input, const double shape);
  double squareDist(const double, const double);
};

inline double emmetDistortion::fastatan(double x) {
    return (x / (1.0 + 0.28 * (x * x))); //
}

inline double emmetDistortion::tanhDist(double input, const double shape) {
  return (1.0 / tanh(shape)) * tanh(input * (shape/2))/ 1.8;
  //return input;
}

inline double emmetDistortion::atanDist(double input, const double shape) {
  return (1.0 / atan(shape)) * atan(input * (shape/2)) /1.8;
}

inline double emmetDistortion::fastAtanDist(double input, const double shape) {
    return (1.0 / fastatan(shape)) * fastatan(input * (shape/2)) / 2.4;
}

inline double emmetDistortion::sigmoidDist(double input, const double shape) {
  return cbrt(input*(shape/4.))/3.;
}

inline double emmetDistortion::squareDist(double input, double shape) {
  return sqrt(input*shape);
}

#endif