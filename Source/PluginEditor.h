/*
  ==============================================================================

 PluginEditor.h
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes
 
 The editor encapsulates the entire user interface of this application. It
 interacts with the Processor, rather than interacting with voices or hardware
 directly. each slider has a value tree slider/combobox attachment to connect to
 the processor to handle changes in DSP effects. All sliders and combo boxes have
 labels attached to them. There is a background image, a scope component, a static
 scale value depending on your device and a styling class called lookAndFeel for
 aesthetic choices about the sliders and combo boxes
  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomFeel.h"

class SynthFrameworkAudioProcessorEditor  : public AudioProcessorEditor,
  public Slider::Listener, public ComboBox::Listener {
public:
  SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor&);
  ~SynthFrameworkAudioProcessorEditor();
  void paint (Graphics&) override;
  void resized() override;
  void sliderValueChanged(Slider*) override;
  void comboBoxChanged(ComboBox*) override;
private:
  SynthFrameworkAudioProcessor& processor; //accesses the processor object that created the editor
  ComboBox osc1Gui, osc2Gui, distGui; //GUI for synth oscillators
  Slider attackSlider, decaySlider, sustainSlider, releaseSlider, osc2PitchSlider, noiseGainSlider,
    filterCutSlider, osc2GainSlider, filterResSlider, lfoFilterIntenSlider, lfoFilterRateSlider,
    /*lfoPitchIntenSlider, lfoPitchRateSlider,*/ compressRatioSlider, compressThreshSlider,
    compressAtkSlider, compressRelSlider, distortionDriveSlider, delayTimeSlider, delayFdbkSlider,
    delayGainSlider, compressGainSlider, totalGainSlider; //all our GUI sliders
  Label asLabel, dcLabel, ssLabel, relLabel, o2gLabel, o2pLabel, o1Label, o2Label, nzLabel,
    fcLabel, frLabel, tgLabel, crLabel, ctLabel, caLabel, clLabel, dtLabel, dfLabel, dgLabel, cgLabel, dLabel, ddLabel,
    lfiLabel, lfrLabel /*, lpiLabel, lprLabel*/; //labels for all our sliders
  
  void addAllGUIComponents();
      
  std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>  osc1GuiAttach, osc2GuiAttach, distGuiAttach;
  std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> //attaches our sliders to their corresponding value tree
    attackSliderAttach, decaySliderAttach, sustainSliderAttach, releaseSliderAttach, noiseGainSliderAttach,
    osc2GainSliderAttach, osc2PitchSliderAttach, filtCutSliderAttach, filtResSliderAttach,
    lfoFilterIntenSliderAttach, lfoFilterRateSliderAttach, /*lfoPitchIntenSliderAttach, lfoPitchRateSliderAttach,*/
    totalGainSliderAttach, compressRatioSliderAttach, compressThreshSliderAttach, 
    compressAtkSliderAttach, compressRelSliderAttach, delayTimeSliderAttach,
    delayFdbkSliderAttach, compressGainSliderAttach, distortionDriveSliderAttach, delayGainSliderAttach;  

  ScopeComponent<float> scopeComponent; //this is the oscilloscope UIwidget
  //MidiKeyboardComponent keyboardComponent;
  //MidiKeyboardState keyboardState;
  CustomFeel lookAndFeel; //defines drawing of sliders combo boxes and fonts
  juce::Image image; //background image
  juce::JPEGImageFormat* j; //necessary to convert background image format
  static float scale; //affects scale of the entire window
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthFrameworkAudioProcessorEditor)
};
