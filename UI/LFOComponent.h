/*
  ==============================================================================

    LFOComponent.h
    Created: 3 Dec 2023 1:13:40pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOComponent  : public juce::Component
{
public:
    LFOComponent();
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
