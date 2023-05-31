/*
  ==============================================================================

    NoteToColor.cpp
    Created: 31 May 2023 12:40:48pm
    Author:  gelli

  ==============================================================================
*/
#include "NoteToColor.h"

NoteColors frequencyToColor(float freq)
{
    // these loops mean note presses between A3 and A4
    // will not need to iterate. within a reasonable bound
    // of what typical users will need in low and high frequencies
    // should in reasonable worst-case scenarios it will take 4
    // extra operations to update the color. for example: 
    // (F#8) 6 kHz -> 3 kHz - > 1.5 kHz -> 750 Hz -> 375 Hz
    // (G0) 24 Hz -> 48 Hz -> 96 Hz -> 192 Hz -> 384 Hz
    while (freq > 440.f)
        freq *= 0.5;
    while (freq < 220.f)
        freq *= 2.;

    if (freq > 214.f && freq < 227.f) return NoteColors::BLACK;
    else if (freq > 227.f && freq < 240.f) return NoteColors::BLUE;
    else if (freq > 240.f && freq < 254.f) return NoteColors::PURPLE;
    else if (freq > 254.f && freq < 269.f) return NoteColors::WHITE;
    else if (freq > 269.f && freq < 285.f) return NoteColors::SEAFOAM;
    else if (freq > 285.f && freq < 302.f) return NoteColors::GREEN;
    else if (freq > 302.f && freq < 320.f) return NoteColors::YELLOWGREEN;
    else if (freq > 320.f && freq < 339.f) return NoteColors::YELLOW;
    else if (freq > 339.f && freq < 359.f) return NoteColors::ORANGE;
    else if (freq > 359.f && freq < 380.f) return NoteColors::REDORANGE;
    else if (freq > 380.f && freq < 403.f) return NoteColors::RED;
    else if (freq > 403.f && freq < 427.f) return NoteColors::DARKRED;
    else if (freq > 427.f && freq < 453.f) return NoteColors::BLACK;
    return NoteColors::WHITE;
}