/*
  ==============================================================================

    SynthSound.h
    Created: 22 Nov 2023 12:02:18pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};
