#include "NoteToColor.h"

NoteColors frequencyToColor(float freq) 
{
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