/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OscComponent.h"

//==============================================================================
/**
*/
class SynthTalkAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthTalkAudioProcessorEditor (SynthTalkAudioProcessor&);
    ~SynthTalkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider gainSlider;
    SynthTalkAudioProcessor& audioProcessor;
    OscComponent osc;
    AdsrComponent adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTalkAudioProcessorEditor)
};
