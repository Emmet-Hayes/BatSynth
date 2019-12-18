/*
  CustomFeel.cpp
  BatSynth
  Created by Emmet Hayes on 11/15/19.
 
 this is where i'm styling the feel and aesthetics of the app. we're filling in the
 rotary knobs, drawing the outline and drawing the rectangular dial on the rotaries
 themselves, setting the fonts, filling the background of the combo boxes and popup menus.
 We also need to get the scale from the editor to scale the fonts based on size of the
 window (since in this program we are scaling it to device window size).
*/

#include "CustomFeel.h"

//initialize the scale of the fonts sand the default slider color
CustomFeel::CustomFeel() {
  scale = 1.f; //default scale
  setColour(Slider::thumbColourId, Colours::darkmagenta); //default slider color
}

//drawing each slider to have this kind of style, filled in with solid color and outlined with a dial
void CustomFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
  double radius = jmin(width / 2, height / 2) - 4.0f;
  double centreX = x + width * 0.5f;
  double centreY = y + height * 0.5f;
  double rx = centreX - radius;
  double ry = centreY - radius;
  double rw = radius * 2.0f;
  double angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  //fill the rotary
  g.setColour(Colours::whitesmoke);
  g.setOpacity(0.13f);
  g.fillEllipse(rx, ry, rw, rw);
  //outline the rotary
  g.setColour(Colours::darkmagenta);
  g.drawEllipse(rx, ry, rw, rw, 0.5f);
  //draw the knob dial
  Path p;
  double pointerLength = radius * 0.33f;
  double pointerThickness = 5.0f;
  p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
  p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY)); //animate
  g.setColour(Colours::seashell); //color of dial
  g.fillPath(p);
}

//draws the combo box default and fills it with the rgb color
void CustomFeel::drawComboBox(Graphics& g, int w, int h, bool isDown, int bx, int by, int bw, int bh,
  ComboBox& cb) {
  Colour c(30, 8, 33);
  g.setColour(c);
  g.fillAll();
}

Font CustomFeel::getComboBoxFont(ComboBox & c) {
  return getCommonMenuFont(scale);
}

Font CustomFeel::getPopupMenuFont() {
  return getCommonMenuFont(scale);
}

//both combo boxes and popup menus will get this font right now.
Font CustomFeel::getCommonMenuFont(float s) {
  return Font("Lucida Console", 10.f * s, Font::bold);
}

//same background as the combo box
void CustomFeel::drawPopupMenuBackground(Graphics& g, int width, int height) {
  Colour c(30, 8, 33);
  g.setColour(c);
  g.fillAll();
}
