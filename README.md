# BatSynth
A polyphonic synthesizer implemented in C++ (with JUCE).
An external midi device (like a usb keyboard) is required to make sounds! 
If you are running the standalone version, any MIDI devices connected to your computer should be automatically detected thanks to the JUCE audio plugin client and show up in the options menu in the top left corner.

I have recently updated this project to feature color changes across the GUI based on the lowest frequency note being currently played. Each bass frequency from the standard 12-note Western-European equal-temperament system will illuminate the UI in its own unique color. The amplitude of the final signal is furthur expressed visually by the brightness of the outlines around all sliders present in the GUI.

This synthesizer has two sound oscillators, each with thirteen different wave shapes.
Click on the label boxes below Osc1 and Osc2 to adjust their current respective oscillator wave shapes. 
There is also a white noise oscillator that can be attenuated to your liking under the noise label.

There are Attack, Decay, Sustain, and Release controls that affect the time-based tonality of the synthesizer.
Attack determines how quickly the oscillator's wave shape reaches full volume
Decay determines how quickly the oscillator decays to the sustain ratio (relative to full volume)
Sustain determines the ratio to full volume of the sound after the decay time has passed
Release determines how quickly the oscillator decays to silence

After the ADSR circuit in the signal chain, there is a 12db per octave low pass resonant filter with
a cutoff frequency parameter to determine what audio frequency to begin filtering, and a resonance
control to determine how resonant the cutoff frequency will be in the final result (be careful!!)

![alt text](Images/BatSynthExampleImage.png)
