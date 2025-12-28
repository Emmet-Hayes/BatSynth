#pragma once
#include "BatSynthDSP.h"

class BatSynthEnv
{
public:
    double adsr(double input, int trigger);
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