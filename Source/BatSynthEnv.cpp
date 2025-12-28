#include "BatSynthEnv.h"


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
    attack = 1 - pow(0.01, 1.0 / (attackMS * BatSynthDSP::sampleRate * 0.001));
}

void BatSynthEnv::setRelease(double releaseMS)
{
    release = pow(0.01, 1.0 / (releaseMS * BatSynthDSP::sampleRate * 0.001));
}

void BatSynthEnv::setSustain(double sustainL)
{
    sustain = sustainL;
}

void BatSynthEnv::setDecay(double decayMS)
{
    decay = pow(0.01, 1.0 / (decayMS * BatSynthDSP::sampleRate * 0.001));
}