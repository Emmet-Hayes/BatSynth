/*
  ==============================================================================

    NoteToColor.h
    Created: 31 May 2023 12:03:25pm
    Author:  Emmet Hayes

  ==============================================================================
*/
#pragma once
#include <unordered_map>
#include "../JuceLibraryCode/JuceHeader.h"

enum class NoteColors {
    BLACK, BLUE, PURPLE, WHITE, SEAFOAM, GREEN,
    YELLOWGREEN, YELLOW, ORANGE, REDORANGE, RED, DARKRED
};

class NoteColorListener {
public:
    virtual void noteColorChanged(NoteColors newColor) = 0;
};

static std::unordered_map<NoteColors, juce::Colour> noteToColorMap{
	{ NoteColors::BLACK, juce::Colour::fromRGB(40, 0, 40) },
	{ NoteColors::BLUE, juce::Colour::fromRGB(0, 0, 255) },
	{ NoteColors::PURPLE, juce::Colour::fromRGB(128, 0, 255) },
	{ NoteColors::WHITE, juce::Colour::fromRGB(255, 255, 255) },
	{ NoteColors::SEAFOAM, juce::Colour::fromRGB(84, 255, 159) },
	{ NoteColors::GREEN, juce::Colour::fromRGB(0, 128, 0) },
	{ NoteColors::YELLOWGREEN, juce::Colour::fromRGB(154, 205, 50) },
	{ NoteColors::YELLOW, juce::Colour::fromRGB(255, 255, 0) },
	{ NoteColors::ORANGE, juce::Colour::fromRGB(255, 165, 0) },
	{ NoteColors::REDORANGE, juce::Colour::fromRGB(255, 69, 0) },
	{ NoteColors::RED, juce::Colour::fromRGB(255, 0, 0) },
	{ NoteColors::DARKRED, juce::Colour::fromRGB(162, 32, 72) }
};

NoteColors frequencyToColor(float freq);