/*
  ==============================================================================

    VoiceComponent.cpp
    Created: 24 Nov 2023 1:56:22pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VoiceComponent.h"

//==============================================================================
VoiceComponent::VoiceComponent(juce::AudioProcessorValueTreeState& apvts) : osc (apvts), adsr(apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(adsr);
    addAndMakeVisible(osc);
    setVoice(0);
}

VoiceComponent::~VoiceComponent()
{
}

void VoiceComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black);   // clear the background
}

void VoiceComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    const auto modulePadding = 20;
    osc.setBounds(modulePadding, 0, 200, 200);
    adsr.setBounds(osc.getRight() + modulePadding, 0, 200, 200);
}

void VoiceComponent::setVoice(int voice) {
    adsr.setVoice(voice);
    osc.setVoice(voice);
}
