/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTalkAudioProcessorEditor::SynthTalkAudioProcessorEditor (SynthTalkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc (audioProcessor.apvts, "OSC1WAVETYPE"), adsr(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (400, 300);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
//
//    oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);
    
    addAndMakeVisible(adsr);
    addAndMakeVisible(osc);
    
}

SynthTalkAudioProcessorEditor::~SynthTalkAudioProcessorEditor()
{
}

//==============================================================================
void SynthTalkAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthTalkAudioProcessorEditor::resized()
{
    adsr.setBounds(getLocalBounds());
    osc.setBounds(10, 10, 100, 30);
}
