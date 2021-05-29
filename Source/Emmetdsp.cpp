
/*
 *  emmetdsp.cpp
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
 *  Edited refactored and repurposed by Emmet Hayes on 9/8/2019.
 *  Thanks to the Goldsmiths Creative Computing Team.
 *    Special thanks to Arturo Castro for the PortAudio implementation.
 *
 *  All the core DSP algorithms live here and in the header file.
 *  Most of it is documented :)
 */

#include "Emmetdsp.h"

//This used to be important for dealing with multichannel playback
float chandiv= 1;
int emmetSettings::sampleRate = 44100;

//When you create an oscillator, the constructor sets the phase of the oscillator to 0.
emmetOsc::emmetOsc(){
  phase = 0.0;
}

//White Noise - always the same unless you seed it.
double emmetOsc::noise() {
  output=(rand()/(float)RAND_MAX)*2-1;
  return(output);
}

//This allows you to set the phase of the oscillator to anything you like.
void emmetOsc::phaseReset(double phaseIn) {
  phase=phaseIn;
}

//generate sine wave using phase math
double emmetOsc::sinewave(double frequency) {
  output=sin(phase*(TWOPI)); //start at 0 generate sine
  if ( phase >= 1.0 ) phase -= 1.0; //tick back phase after full period
  phase += (1./(emmetSettings::sampleRate/(frequency))); //move phase forward
  return(output);
}

//cosine oscillator, it just starts different place than sine wave
double emmetOsc::coswave(double frequency) {
  output=cos(phase*(TWOPI));
  if (phase >= 1.0) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  return(output);
}

//This is a square wave
double emmetOsc::square(double frequency) {
  if (phase <= 0.5) output=-1; //halfway through the phase journey
  if (phase > 0.5) output=1; //make square shape
  if (phase >= 1.0) phase -= 1.0; //phase reset if its done
  phase += (1./(emmetSettings::sampleRate/(frequency))); //tick
  return(output);
}

//This is a pulse generator that creates a signal between -1 and 1.
//set duty between 0 and 1 to manipulate the wave
double emmetOsc::pulse(double frequency, double duty) {
  if (duty<0.) duty=0;
  if (duty>1.) duty=1;
  if ( phase >= 1.0 ) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  if (phase<duty) output=-1.;
  if (phase>duty) output=1.;
  return(output);
}

//Sawtooth generator. This is like a phasor but goes between -1 and 1
double emmetOsc::saw(double frequency) {
  output=phase*2 - 1;
  if ( phase >= 1.0 ) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  return(output);
}

//reverse sawtooth
double emmetOsc::sawr(double frequency) {
  return -saw(frequency);
}

//calls sinewave and pre-distorts to give it a harmonic texture
double emmetOsc::softdistsine(double frequency) {
  return tanh(1.2*sinewave(frequency));
}

//This is a distorted sinewave
double emmetOsc::harddistsine(double frequency) {
  double s = 1.3 * (sinewave(frequency));
  if (s > 1) s = 1;
  if (s < -1) s = -1;
  return s;
}

//This is a triangle wave.
double emmetOsc::triangle(double frequency) {
  if ( phase >= 1.0 ) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/frequency));
  if (phase <= 0.5 ) output =(phase - 0.25) * 4.;
  else output =((1.0-phase) - 0.25) * 4.;
  return(output);
}

//this is an exponential wavetype shaped more sharply than regular saw
double emmetOsc::sawwane(double frequency) {
  output = phase * phase * 2 - 1;
  if (phase >= 1.0) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  return(output);
}

//this is an exponential wavetype shaped more toothy than regular saw
double emmetOsc::sawwax(double frequency) {
  output = sqrt(phase) * 2 - 1;
  if (phase >= 1.0) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  return(output);
}

//combine pulse algorithm with saw give it a duty between 0 and 0.4
double emmetOsc::sawpulse(double frequency, double duty) {
  if (duty<0.) duty=0;
  else if (duty>0.4) duty=0.4;
  if (phase < duty) output = -1;
  else if (phase > (1 - duty)) output = 1;
  else output = phase * 2 - 1;
  if (phase >= 1.0) phase -= 1.0;
  phase += (1./(emmetSettings::sampleRate/(frequency)));
  return output;
}

//initializing memory set for delay
emmetFractionalDelay::emmetFractionalDelay ( void ) {
  memset( memory, 0, delaySize*sizeof (double) );
}

//
double emmetFractionalDelay::dl ( double sig, double delayTime, double fdback ) {
  time = fmin(fabs(delayTime), delaySize); //Set delay time can't go over size with fmin
  int32_t delay = time; // Truncated
  double fractAmount = time - delay; // Fractional remainder, should be like 0.001 or something
  double truncAmount = 1.0f - fractAmount; // Inverse fractional remainder should be -0.001
  feedback = fdback;
  // Update read pointer
  readPointer = writePointer - delay; //very nice :)
  if (readPointer < 0) //if delay is longer than writePointer
    readPointer += delaySize; //move read up a 88200 (2 secs at 44.1k or 1 sec at 88.2k)
  
  int readPointerFractPart = readPointer-1;  //last slot memory at 88199. this will index memory[]
  if (readPointerFractPart < 0) //if readPointer is somehow negative
    readPointerFractPart += delaySize; //move forward (88200)
  
  // Get interpolated sample
  double y = memory[readPointer] * truncAmount + memory[readPointerFractPart] * fractAmount;
    
  // Write new sample
  memory[writePointer] = y * feedback + sig;
    
  // Increment write pointer
  if (++writePointer >= delaySize)
    writePointer -= delaySize; //go back to zero once writePointer hits 88200
  return y;
}

//I particularly like these. cutoff between 0 and 1
double emmetFilter::lopass(double input, double cutoff) {
  output=outputs[0] + cutoff*(input-outputs[0]);
  outputs[0]=output;
  return(output);
}

//as above
double emmetFilter::hipass(double input, double cutoff) {
  output=input-(outputs[0] + cutoff*(input-outputs[0]));
  outputs[0]=output;
  return(output);
}
//awesome. cuttof is freq in hz. res is between 1 and whatever. Watch out!
double emmetFilter::lores(double input,double cutoff1, double resonance) {
  cutoff=cutoff1;
  if (cutoff<20) cutoff=20; //don't accept a value lower than 20hz no matter what
  if (cutoff>(emmetSettings::sampleRate/2.22)) cutoff=(emmetSettings::sampleRate/2.22);
  if (resonance<1. || cutoff1 < 225) resonance = 1.; //careful with this number, don't blow your speakers.
  z=cos(TWOPI*cutoff/emmetSettings::sampleRate); //filter math
  c=2-2*z;
  double r=(sqrt(2.0)*sqrt(-pow((z-1.0),3.0))+resonance*(z-1))/(resonance*(z-1));
  x=x+(input-y)*c;
  y=y+x;
  x=x*r;
  output=y; //lores
  return(output);
}

//working hires filter
double emmetFilter::hires(double input,double cutoff1, double resonance) {
  cutoff=cutoff1;
  if (cutoff<10) cutoff=10;
  if (cutoff>(emmetSettings::sampleRate)) cutoff=(emmetSettings::sampleRate);
  if (resonance<1.) resonance = 1.;
  z=cos(TWOPI*cutoff/emmetSettings::sampleRate);
  c=2-2*z;
  double r=(sqrt(2.0)*sqrt(-pow((z-1.0),3.0))+resonance*(z-1))/(resonance*(z-1));
  x=x+(input-y)*c;
  y=y+x;
  x=x*r;
  output=input-y; //hires
  return(output);
}

//This works a bit. Needs attention.
double emmetFilter::bandpass(double input,double cutoff1, double resonance) {
  cutoff=cutoff1;
  if (cutoff>(emmetSettings::sampleRate*0.5)) cutoff=(emmetSettings::sampleRate*0.5);
  if (resonance>=1.) resonance=0.999999;
  z=cos(TWOPI*cutoff/emmetSettings::sampleRate);
  inputs[0] = (1-resonance)*(sqrt(resonance*(resonance-4.0*pow(z,2.0)+2.0)+1));
  inputs[1] = 2*z*resonance;
  inputs[2] = pow((resonance*-1),2);
  
  output=inputs[0]*input+inputs[1]*outputs[1]+inputs[2]*outputs[2];
  outputs[2]=outputs[1];
  outputs[1]=output;
  return(output);
}

/* OK this compressor and gate are now ready to use. The envelopes, like all the envelopes in this recent update,
 use stupid algorithms for incrementing - consequently a long attack is something like 0.0001 and a long release is
 like 0.9999. Annoyingly, a short attack is 0.1, and a short release is 0.99. I'll sort this out laters */
double emmetDyn::gate(double input, double threshold, long holdtime, double attack, double release) {
  if (fabs(input)>threshold && attackphase!=1){  //attack phase activated
    holdcount=0;
    releasephase=0;
    attackphase=1;
    if(amplitude==0) amplitude=0.01;
  }
  
  if (attackphase == 1 && amplitude < 1) { //if attack phase activated, attenuate amplitude
    amplitude*=(1+attack); //amplitude change scaled by attack speed
    output=input*amplitude; //output amplified attack signal from the gate
  }
  
  if (amplitude >= 1) { //actiavte the hold phase if amplitude is greater than 1
    attackphase=0; //out of attack phase
    holdphase=1; //and into the hold phase
  }
  
  if (holdcount < holdtime && holdphase == 1) { //if holdtime is triggered allow output to pass through
    output=input; //passing input to output unaltered
    holdcount++; //increment the timer to hit the hold time parameter for the next phase
  }
  
  if (holdcount >= holdtime) { //if hold time is reached enter release phase
    holdphase=0; //leaving the hold phase
    releasephase=1; //into release phase
  }
  
  if (releasephase==1 && amplitude > 0.) { //if release phase is reached and amplitude is nonzero
    output=input*(amplitude*=release); //output a tail of the signal based on the release time
  }
  
  return output;
}

//This thing is great! goes through phases similar to the gate.
double emmetDyn::compressor(double input, double ratio, double threshold, double attack, double release) {
  if (fabs(input) > threshold && attackphase != 1){ // go into attack phase if input hits threshold
    releasephase = 0;
    attackphase = 1;
    currentRatio = 1;
  }
  
  if (attackphase == 1 && currentRatio < ratio-1) { // update currentRatio based on attack speed in atk phase
    currentRatio *= 1 + (1 / attack);
  }
  
  if (currentRatio >= ratio-1 && fabs(input) < threshold ){ // if ratio hits threshold
    attackphase = 0;
    releasephase = 1;
  }
  
  if (releasephase == 1 && currentRatio > 0.) {
    currentRatio /= 1 + (1 / release);
  }
  output = input/(1.+currentRatio);
  
  return output;
}

double emmetDyn::limiter(double input) {
  return (input > 0.95) ? 0.95 : (input < -0.95) ? -0.95 : input;
}

// adsr. It's not bad, very simple to use!
double emmetEnv::adsr(double input, int trigger) {
  if (trigger==1 && attackphase!=1 && holdphase!=1 && decayphase!=1){
    holdcount=0;
    decayphase=0;
    sustainphase=0;
    releasephase=0;
    attackphase=1;
  }
    
  if (attackphase==1) {
    releasephase=0;
    amplitude+=(1*attack);
    output=input*amplitude;
    if (amplitude>=1) {
      amplitude=1;
      attackphase=0;
      decayphase=1;
    }
  }
  
  if (decayphase==1) {
    output=input*(amplitude*=decay);
    if (amplitude<=sustain) {
      decayphase=0;
      holdphase=1;
    }
  }
    
  if (holdcount<holdtime && holdphase==1) {
    output=input*amplitude;
    holdcount++;
  }
    
  if (holdcount>=holdtime && trigger==1) {
    output=input*amplitude;
  }
    
  if (holdcount>=holdtime && trigger!=1) {
    holdphase=0;
    releasephase=1;
  }
    
  if (releasephase==1 && amplitude>0.) {
    output=input*(amplitude*=release);
  }
  return output;
}

void emmetEnv::setAttack(double attackMS) {
  attack = 1-pow( 0.01, 1.0 / ( attackMS * emmetSettings::sampleRate * 0.001 ) );
}

void emmetEnv::setRelease(double releaseMS) {
  release = pow( 0.01, 1.0 / ( releaseMS * emmetSettings::sampleRate * 0.001 ) );
}

void emmetEnv::setSustain(double sustainL) {
  sustain = sustainL;
}

void emmetEnv::setDecay(double decayMS) {
  decay = pow( 0.01, 1.0 / ( decayMS * emmetSettings::sampleRate * 0.001 ) );
}

void emmetDyn::setAttack(double attackMS) {
  attack = pow( 0.01, 1.0 / ( attackMS * emmetSettings::sampleRate * 0.001 ) );
}

void emmetDyn::setRelease(double releaseMS) {
  release = pow( 0.01, 1.0 / ( releaseMS * emmetSettings::sampleRate * 0.001 ) );
}

void emmetDyn::setThreshold(double thresholdI) {
  threshold = thresholdI;
}

void emmetDyn::setRatio(double ratioF) {
  ratio = ratioF;
}
