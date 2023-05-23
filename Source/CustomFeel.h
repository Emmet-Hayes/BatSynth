/* CustomFeel.h
 
  created by Emmet Hayes 11/3/19
 
 This class is what styles the rotaries and combo boxes of the synthesizer.
 The rotaries and combo boxes have their draw functions defined here, as well as
 Font settings and scale member variable to affect the size of each widget in
 the window.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CustomFeel : public LookAndFeel_V4 
{
public:
    CustomFeel();
    
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
    
    void drawComboBox(Graphics& g, int w, int h, bool isDown, int bx, int by, int bw, int bh, ComboBox& cb) override;
    
    void drawPopupMenuBackground(Graphics& g, int width, int height) override;
    
    Font getComboBoxFont(ComboBox& c) override;
    
    Font getPopupMenuFont() override;
    
    void setScale(float s) { scale = s; }

private:
    Font getCommonMenuFont(float s); //returns a default font
    
    float scale;
};
