#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound 
{
public:
	bool appliesToNote(int) { return true; }
	bool appliesToChannel(int) { return true; }
};
