#include "BatSynthDSP.h"

float chandiv = 1;
int BatSynthSettings::sampleRate = 44100;
const int oversampleFactor = 2;

double BatSynthOsc::noise() 
{
	output = (rand()/(float)RAND_MAX) * 2 - 1;
	return output;
}

void BatSynthOsc::phaseReset(double phaseIn) 
{
	phase = phaseIn;
}

double BatSynthOsc::sinewave(double freq) 
{
	output = sin(phase * (TWOPI));
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(BatSynthSettings::sampleRate/(freq)));
	return output;
}

double BatSynthOsc::coswave(double freq) 
{
	output = cos(phase * (TWOPI));
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(BatSynthSettings::sampleRate/(freq)));
	return output;
}

double BatSynthOsc::square(double freq) 
{
    double oversampledOutput[oversampleFactor];
	
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
        if (phase <= 0.5) oversampledOutput[i] = -1;
	    else oversampledOutput[i] = 1;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

double BatSynthOsc::pulse(double freq, double duty) 
{
    if (duty < 0.) duty = 0;
	if (duty > 1.) duty = 1;
    
	double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1. /(BatSynthSettings::sampleRate / freq));
    	if (phase >= 1.0) phase -= 1.0;
    	if (phase <= duty) oversampledOutput[i] = -1.;
	    else oversampledOutput[i] = 1.;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

double BatSynthOsc::saw(double freq) 
{
    double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
        oversampledOutput[i] = phase * 2 - 1;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

double BatSynthOsc::sawr(double freq) 
{
	return -saw(freq);
}

double BatSynthOsc::softdistsine(double freq) 
{
    double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
        oversampledOutput[i] = tanh(1.2 * sinewave(freq));
        
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

double BatSynthOsc::harddistsine(double freq) 
{
    double oversampledOutput[oversampleFactor];

    for (int i = 0; i < oversampleFactor; ++i) {
        oversampledOutput[i] = 1.3 * sinewave(freq);
	    if (oversampledOutput[i] > 1) oversampledOutput[i] = 1;
	    if (oversampledOutput[i] < -1) oversampledOutput[i] = -1;
    }

    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);

    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);

    output = sum / oversampleFactor;

	return output;
}

double BatSynthOsc::triangle(double freq) 
{
    double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
	    if (phase <= 0.5) oversampledOutput[i] = (phase - 0.25) * 4.;
	    else oversampledOutput[i] = ((1.0 - phase) - 0.25) * 4.;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;

	return output;
}

double BatSynthOsc::sawwane(double freq) 
{
    double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
        oversampledOutput[i] = phase * phase * 2 - 1;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

double BatSynthOsc::sawwax(double freq) 
{
    double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
        oversampledOutput[i] = sqrt(phase) * 2 - 1;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
    return output;
}

double BatSynthOsc::sawpulse(double freq, double duty) 
{
	if (duty < 0.) duty = 0;
	else if (duty > 0.4) duty = 0.4;
	
	double oversampledOutput[oversampleFactor];
    
    for (int i = 0; i < oversampleFactor; ++i)
    {
        phase += (1./ (BatSynthSettings::sampleRate * oversampleFactor / freq));
        if (phase >= 1.0 ) phase -= 1.0;
	    if (phase < duty) oversampledOutput[i] = -1;
	    else if (phase > (1 - duty)) oversampledOutput[i] = 1;
	    else oversampledOutput[i] = phase * 2 - 1;
    }
    
    BatSynthLPFilter lpFilter;
    lpFilter.setCutoffFrequency(BatSynthSettings::sampleRate * 0.48);
    
    double sum = 0;
    for (int i = 0; i < oversampleFactor; ++i)
        sum += lpFilter.process(oversampledOutput[i]);
        
    output = sum / oversampleFactor;
    
	return output;
}

float BatSynthDelayLine::process(float input) 
{
    float delayedSignal = read(time);
    write(input + delayedSignal * feedback);
    return delayedSignal;
}

void BatSynthDelayLine::write(float input)
{
    buffer[writePointer++] = input;
    if (writePointer >= MAX_DELAY)
        writePointer = 0;
}

float BatSynthDelayLine::read(float delayTime)
{
    float readPointer = float(writePointer) - time;
    while (readPointer < 0) readPointer += MAX_DELAY;
        
    int index0 = int(readPointer);
    int index1 = (index0 + 1) % MAX_DELAY;
        
    float frac = readPointer - float(index0);
    float val0 = buffer[index0];
    float val1 = buffer[index1];
        
    return val0 + frac * (val1 - val0);
}

double BatSynthFilter::lopass(double in, double cutoff) 
{
	output = outputs[0] + cutoff*(in-outputs[0]);
	outputs[0] = output;
	return output;
}

double BatSynthFilter::hipass(double in, double cutoff) 
{
	output = input - (outputs[0] + cutoff * (in - outputs[0]));
	outputs[0] = output;
	return output;
}

double BatSynthFilter::lores(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff < 20) cutoff = 20;
	if (cutoff > (BatSynthSettings::sampleRate / 2.22)) cutoff = (BatSynthSettings::sampleRate / 2.22);
	if (resonance < 1. || cutoff1 < 225) resonance = 1.;
	z = cos(TWOPI * cutoff / BatSynthSettings::sampleRate);
	c = 2 - 2 * z;
	double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1))/(resonance * (z - 1));
	x = x + (in - y) * c;
	y = y + x;
	x = x * r;
	output = y;
	return output;
}

double BatSynthFilter::hires(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff < 10) cutoff = 10;
	if (cutoff > (BatSynthSettings::sampleRate)) cutoff = (BatSynthSettings::sampleRate);
	if (resonance < 1.) resonance = 1;
	z = cos(TWOPI * cutoff / BatSynthSettings::sampleRate);
	c= 2 - 2 * z;
	double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1))/(resonance * (z - 1));
	x = x + (in - y) * c;
	y = y + x;
	x = x * r;
	output = in - y;
	return output;
}

double BatSynthFilter::bandpass(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff > (BatSynthSettings::sampleRate * 0.5)) cutoff = (BatSynthSettings::sampleRate * 0.5);
	if (resonance >= 1.) resonance = 0.999999;
	z = cos(TWOPI * cutoff / BatSynthSettings::sampleRate);
	inputs[0] = (1 - resonance) * (sqrt(resonance * (resonance - 4.0 * pow(z, 2.0) + 2.0) + 1));
	inputs[1] = 2 * z * resonance;
	inputs[2] = pow((resonance * -1), 2);
	
	output = inputs[0] * in + inputs[1] * outputs[1] + inputs[2] * outputs[2];
	outputs[2] = outputs[1];
	outputs[1] = output;
	return output;
}

double BatSynthCompressor::process(double input)
{
    double inputLevel = fabs(input);
        
    if (inputLevel > threshold) {
        double desiredGain = threshold / inputLevel;
        double dbDesiredGain = log10(desiredGain) * 20.0;
        double dbGainReduction = dbDesiredGain * (1.0 - 1.0 / ratio);
        double desiredEnvelope = pow(10.0, dbGainReduction / 20.0);
            
        if (desiredEnvelope < envelope) 
        {
            double coeff = exp(log(0.01) / (attackTime * BatSynthSettings::sampleRate));
            envelope = coeff * envelope + (1.0 - coeff) * desiredEnvelope;
        }
        else 
        {
            double coeff = exp(log(0.01) / (releaseTime * BatSynthSettings::sampleRate));
            envelope = coeff * envelope + (1.0 - coeff) * desiredEnvelope;
        }
    } 
    else
    {
        envelope = 1.0;
    }
    
    gain = envelope;
    return input * gain;
}

double BatSynthEnv::adsr(double in, int trigger) 
{
	if (trigger == 1 && attackphase != 1 && holdphase != 1 && decayphase != 1)
	{
		holdcount = 0;
		decayphase = 0;
		sustainphase = 0;
		releasephase = 0;
		attackphase = 1;
	}
		
	if (attackphase == 1) 
	{
		releasephase = 0;
		amplitude += (1 * attack);
		output = in * amplitude;
		if (amplitude >= 1) {
			amplitude = 1;
			attackphase = 0;
			decayphase = 1;
		}
	}
	
	if (decayphase == 1) 
	{
		output = in * (amplitude *= decay);
		if (amplitude <= sustain) 
		{
			decayphase = 0;
			holdphase = 1;
		}
	}
		
	if (holdcount < holdtime && holdphase == 1) 
	{
		output = in * amplitude;
		holdcount++;
	}
		
	if (holdcount >= holdtime && trigger == 1)
		output = in * amplitude;
		
	if (holdcount >= holdtime && trigger != 1) 
	{
		holdphase = 0;
		releasephase = 1;
	}
		
	if (releasephase == 1 && amplitude > 0.)
		output = in * (amplitude *= release);

	return output;
}

void BatSynthEnv::setAttack(double attackMS) 
{
	attack = 1 - pow(0.01, 1.0 / (attackMS * BatSynthSettings::sampleRate * 0.001));
}

void BatSynthEnv::setRelease(double releaseMS) 
{
	release = pow(0.01, 1.0 / (releaseMS * BatSynthSettings::sampleRate * 0.001));
}

void BatSynthEnv::setSustain(double sustainL) 
{
	sustain = sustainL;
}

void BatSynthEnv::setDecay(double decayMS) 
{
	decay = pow(0.01, 1.0 / (decayMS * BatSynthSettings::sampleRate * 0.001));
}