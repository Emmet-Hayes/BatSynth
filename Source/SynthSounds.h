/*
  ==============================================================================

    SynthSounds.h
    Created: 3 Sep 2019 11:46:01pm
    Author:  ehaye

 Not much is happening here.  Just implementing pure virtuals that are required
 to allow Sounds to be applied to the SynthVoice class.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//Necessary to implement these virtuals to make sound happen with JUCE
class SynthSound : public SynthesiserSound {
public:
  //pure virtuals for when a note is pressed the sound is applied to note and midi channel
  bool appliesToNote(int /*midiNoteNumber*/) { return true; }
  bool appliesToChannel(int /**/) { return true; }
};
