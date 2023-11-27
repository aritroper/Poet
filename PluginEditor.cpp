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
    
    setSize (460, 300);
    addAndMakeVisible(voice);
    makeVoiceButtons();
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
    voice.setBounds(0, 40, 460, 200);
}

void SynthTalkAudioProcessorEditor::makeVoiceButtons() {
    const int buttonWidth = 60;
    const int buttonHeight = 20;
    const int buttonPadding = 5;
    int x = buttonPadding;

    for (int i = 0; i < audioProcessor.numberOfVoices; ++i)
    {
        auto* button = new juce::TextButton("Voice " + juce::String(i + 1));
        button->setBounds(x, buttonPadding, buttonWidth, buttonHeight);
        button->onClick = [this, i] { loadVoice(i); };
        addAndMakeVisible(button);
        voiceButtons.add(button);

        x += buttonWidth + buttonPadding;
    }
}

void SynthTalkAudioProcessorEditor::loadVoice(int voiceIndex) {
    voice.setVoice(voiceIndex);
}
