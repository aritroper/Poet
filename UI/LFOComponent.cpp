/*
  ==============================================================================

    LFOComponent.cpp
    Created: 3 Dec 2023 1:13:40pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOComponent.h"

//==============================================================================
LFOComponent::LFOComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("LFOComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LFOComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
