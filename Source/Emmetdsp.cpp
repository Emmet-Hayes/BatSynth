#include "Emmetdsp.h"

float chandiv = 1;
int emmetSettings::sampleRate = 44100;

emmetOsc::emmetOsc()
{
	phase = 0.0;
}

double emmetOsc::noise() 
{
	output = (rand()/(float)RAND_MAX) * 2 - 1;
	return (output);
}

void emmetOsc::phaseReset(double phaseIn) 
{
	phase = phaseIn;
}

double emmetOsc::sinewave(double freq) 
{
	output = sin(phase * (TWOPI));
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::coswave(double freq) 
{
	output = cos(phase * (TWOPI));
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::square(double freq) 
{
	if (phase <= 0.5) output =- 1;
	if (phase > 0.5) output = 1;
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::pulse(double freq, double duty) 
{
	if (duty<0.) duty=0;
	if (duty>1.) duty=1;
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	if (phase<duty) output= -1.;
	if (phase>duty) output= 1.;
	return(output);
}

double emmetOsc::saw(double freq) 
{
	output = phase * 2 - 1;
	if (phase>= 1.0 ) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::sawr(double freq) 
{
	return -saw(freq);
}

double emmetOsc::softdistsine(double freq) 
{
	return tanh(1.2 * sinewave(freq));
}

double emmetOsc::harddistsine(double freq) 
{
	double s = 1.3 * (sinewave(freq));
	if (s > 1) s = 1;
	if (s < -1) s = -1;
	return s;
}

double emmetOsc::triangle(double freq) 
{
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/freq));
	if (phase <= 0.5) output = (phase - 0.25) * 4.;
	else output =((1.0 - phase) - 0.25) * 4.;
	return (output);
}

double emmetOsc::sawwane(double freq) 
{
	output = phase * phase * 2 - 1;
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::sawwax(double freq) 
{
	output = sqrt(phase) * 2 - 1;
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return (output);
}

double emmetOsc::sawpulse(double freq, double duty) 
{
	if (duty < 0.) duty = 0;
	else if (duty > 0.4) duty = 0.4;
	if (phase < duty) output = -1;
	else if (phase > (1 - duty)) output = 1;
	else output = phase * 2 - 1;
	if (phase >= 1.0) phase -= 1.0;
	phase += (1./(emmetSettings::sampleRate/(freq)));
	return output;
}

emmetFractionalDelay::emmetFractionalDelay() 
{
	memset(memory, 0, delaySize * sizeof(double));
}

double emmetFractionalDelay::dl (double sig, double delayTime, double fdback) 
{
	time = fmin(fabs(delayTime), delaySize);
	int32_t delay = time;
	double fractAmount = time - delay;
	double truncAmount = 1.0f - fractAmount;
	feedback = fdback;
	readPointer = writePointer - delay;
	if (readPointer < 0)
		readPointer += delaySize;
	
	int readPointerFractPart = readPointer-1;
	if (readPointerFractPart < 0)
		readPointerFractPart += delaySize;
	
	double y = memory[readPointer] * truncAmount + memory[readPointerFractPart] * fractAmount;
	memory[writePointer] = y * feedback + sig;
		
	if (++writePointer >= delaySize)
		writePointer -= delaySize;

	return y;
}

double emmetFilter::lopass(double in, double cutoff) 
{
	output = outputs[0] + cutoff*(in-outputs[0]);
	outputs[0] = output;
	return (output);
}

double emmetFilter::hipass(double in, double cutoff) 
{
	output = input - (outputs[0] + cutoff * (in - outputs[0]));
	outputs[0] = output;
	return (output);
}

double emmetFilter::lores(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff < 20) cutoff = 20;
	if (cutoff > (emmetSettings::sampleRate/2.22)) cutoff = (emmetSettings::sampleRate/2.22);
	if (resonance < 1. || cutoff1 < 225) resonance = 1.;
	z = cos(TWOPI * cutoff/emmetSettings::sampleRate);
	c = 2 - 2 * z;
	double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1))/(resonance * (z - 1));
	x = x + (in - y) * c;
	y = y + x;
	x = x * r;
	output = y;
	return (output);
}

double emmetFilter::hires(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff < 10) cutoff = 10;
	if (cutoff > (emmetSettings::sampleRate)) cutoff = (emmetSettings::sampleRate);
	if (resonance < 1.) resonance = 1.;
	z = cos(TWOPI * cutoff/emmetSettings::sampleRate);
	c= 2 - 2 * z;
	double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1))/(resonance * (z - 1));
	x = x + (in - y) * c;
	y = y + x;
	x = x * r;
	output = in - y;
	return (output);
}

double emmetFilter::bandpass(double in, double cutoff1, double resonance) 
{
	cutoff = cutoff1;
	if (cutoff > (emmetSettings::sampleRate*0.5)) cutoff=(emmetSettings::sampleRate*0.5);
	if (resonance >= 1.) resonance = 0.999999;
	z = cos(TWOPI * cutoff/emmetSettings::sampleRate);
	inputs[0] = (1 - resonance)*(sqrt(resonance * (resonance - 4.0 * pow(z, 2.0) + 2.0) + 1));
	inputs[1] = 2 * z * resonance;
	inputs[2] = pow((resonance * -1), 2);
	
	output=inputs[0] * in + inputs[1] * outputs[1] + inputs[2] * outputs[2];
	outputs[2] = outputs[1];
	outputs[1] = output;
	return (output);
}

double emmetDyn::gate(double in, double threshold, long holdtime, double attack, double release) 
{
	if (fabs(in) > threshold && attackphase != 1)
	{ 
		holdcount = 0;
		releasephase = 0;
		attackphase = 1;
		if(amplitude == 0) amplitude = 0.01;
	}
	
	if (attackphase == 1 && amplitude < 1) 
	{ 
		amplitude *= (1 + attack);
		output = input * amplitude;
	}
	
	if (amplitude >= 1) 
	{
		attackphase = 0;
		holdphase = 1;
	}
	
	if (holdcount < holdtime && holdphase == 1) 
	{
		output = input;
		holdcount++;
	}
	
	if (holdcount >= holdtime) 
	{
		holdphase = 0;
		releasephase = 1;
	}
	
	if (releasephase == 1 && amplitude > 0.)
		output = input * (amplitude *= release);
	
	return output;
}

double emmetDyn::compressor(double in, double ratio, double threshold, double atk, double rel) 
{
	if (fabs(in) > threshold && attackphase != 1)
	{
		releasephase = 0;
		attackphase = 1;
		currentRatio = 1;
	}
	
	if (attackphase == 1 && currentRatio < ratio-1) 
		currentRatio *= 1 + (1 / atk);
	
	if (currentRatio >= ratio - 1 && fabs(in) < threshold )
	{
		attackphase = 0;
		releasephase = 1;
	}
	
	if (releasephase == 1 && currentRatio > 0.) 
		currentRatio /= 1 + (1 / rel);

	output = in / (1. + currentRatio);
	
	return output;
}

double emmetDyn::limiter(double in) 
{
	return (in > 0.95) ? 0.95 : (in < -0.95) ? -0.95 : in;
}

double emmetEnv::adsr(double in, int trigger) 
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

void emmetEnv::setAttack(double attackMS) 
{
	attack = 1 - pow(0.01, 1.0 / (attackMS * emmetSettings::sampleRate * 0.001));
}

void emmetEnv::setRelease(double releaseMS) 
{
	release = pow(0.01, 1.0 / (releaseMS * emmetSettings::sampleRate * 0.001));
}

void emmetEnv::setSustain(double sustainL) 
{
	sustain = sustainL;
}

void emmetEnv::setDecay(double decayMS) 
{
	decay = pow(0.01, 1.0 / (decayMS * emmetSettings::sampleRate * 0.001));
}

void emmetDyn::setAttack(double attackMS) 
{
	attack = pow(0.01, 1.0 / (attackMS * emmetSettings::sampleRate * 0.001));
}

void emmetDyn::setRelease(double releaseMS) 
{
	release = pow(0.01, 1.0 / (releaseMS * emmetSettings::sampleRate * 0.001));
}

void emmetDyn::setThreshold(double thresholdI) 
{
	threshold = thresholdI;
}

void emmetDyn::setRatio(double ratioF) 
{
	ratio = ratioF;
}
