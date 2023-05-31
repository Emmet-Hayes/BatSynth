/* CustomFeel.h
 
  created by Emmet Hayes 11/3/19
 
 This class is what styles the rotaries and combo boxes of the synthesizer.
 The rotaries and combo boxes have their draw functions defined here, as well as
 Font settings and scale member variable to affect the size of each widget in
 the window.
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <unordered_map>
#include "NoteToColor.h"

#ifndef CustomFeel_h
#define CustomFeel_h

class CustomFeel : public LookAndFeel_V4 {
public:
	CustomFeel();
	void drawRotarySlider(Graphics&, int, int, int, int, float, const float, 
						  const float, Slider&) override;
	void drawComboBox(Graphics&, int, int, bool, int, int, int, int, ComboBox&) override;
	void drawPopupMenuBackground(Graphics&, int, int) override;
	Font getComboBoxFont(ComboBox&) override;
	Font getPopupMenuFont() override;
	inline void setScale(const float s) { scale = s; }
	void setGainColorIntensity(float ampl); // map amplitude to color intensity, someday pitch to color
	void setFrequencyColor(float freq);

private:
	Font getCommonMenuFont(float); //returns a default font

	float scale;
	float rotaryOutlineBrightness = 1.0f;
	NoteColors currentNoteColor = NoteColors::WHITE;

};

#endif /* CustomFeel_h */
