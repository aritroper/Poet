/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/VoiceComponent.h"

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
    
    void makeVoiceButtons();
    void loadVoice(int voice);

private:
    juce::OwnedArray<juce::TextButton> voiceButtons;
    juce::Slider gainSlider;
    SynthTalkAudioProcessor& audioProcessor;
    VoiceComponent voice;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTalkAudioProcessorEditor)
};
