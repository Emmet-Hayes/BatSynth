#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <unordered_map>
#include "NoteToColor.h"

class CustomFeel : public LookAndFeel_V4 
{
public:
	CustomFeel();
	void drawRotarySlider(Graphics&, int, int, int, int, float, const float, 
						  const float, Slider&) override;
	void drawComboBox(Graphics&, int, int, bool, int, int, int, int, ComboBox&) override;
	void drawPopupMenuBackground(Graphics&, int, int) override;
	Font getComboBoxFont(ComboBox&) override;
	Font getPopupMenuFont() override;
	inline void setScale(const float s) { scale = s; }
	void setGainColorIntensity(float ampl);
	void setFrequencyColor(float freq);

private:
	Font getCommonMenuFont(float);

	float scale;
	float rotaryOutlineBrightness = 1.0f;
	NoteColors currentNoteColor = NoteColors::WHITE;

};
