/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTalkAudioProcessorEditor::SynthTalkAudioProcessorEditor (SynthTalkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), voice(p.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (460, 800);
    addAndMakeVisible(voice);
}

SynthTalkAudioProcessorEditor::~SynthTalkAudioProcessorEditor()
{
}

//==============================================================================
void SynthTalkAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void SynthTalkAudioProcessorEditor::resized()
{
    voice.setBounds(0, 20, 460, 800);
}
