/*
  ==============================================================================

    VoiceComponent.cpp
    Created: 24 Nov 2023 1:56:22pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VoiceComponent.h"
#include "../Data/SynthData.h"

//==============================================================================
VoiceComponent::VoiceComponent(juce::AudioProcessorValueTreeState& apvts) :
    osc (apvts),
    ampEnvelope(apvts, "AMPATTACK", "AMPDECAY", "AMPSUSTAIN", "AMPRELEASE"),
    filter(apvts),
    modEnvelope(apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    makeOscButtons();
    addAndMakeVisible(ampEnvelope);
    addAndMakeVisible(osc);
    addAndMakeVisible(filter);
    addAndMakeVisible(modEnvelope);
    setOsc(0);
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
    osc.setBounds(modulePadding, 40, 200, 200);
    ampEnvelope.setBounds(osc.getRight() + modulePadding, 40, 200, 200);
    filter.setBounds(modulePadding, osc.getBottom() + modulePadding, 200, 200);
    modEnvelope.setBounds(filter.getRight() + modulePadding, osc.getBottom() + modulePadding, 200, 200);
}


void VoiceComponent::makeOscButtons() {
    const int buttonWidth = 60;
    const int buttonHeight = 20;
    const int buttonPadding = 5;
    int x = 20;

    for (int i = 0; i < NUMBER_OF_OSCILLATORS; ++i)
    {
        auto* button = new juce::TextButton("Osc " + juce::String(i + 1));
        button->setBounds(x, buttonPadding, buttonWidth, buttonHeight);
        button->onClick = [this, i] { setOsc(i); };
        addAndMakeVisible(button);
        oscButtons.add(button);

        x += buttonWidth + buttonPadding;
    }
}

void VoiceComponent::setOsc(int osc) {
    this->ampEnvelope.setOsc(osc);
    this->osc.setOsc(osc);
    this->filter.setOsc(osc);
    this->modEnvelope.setOsc(osc);
}
