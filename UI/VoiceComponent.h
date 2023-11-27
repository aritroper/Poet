/*
  ==============================================================================

    VoiceComponent.h
    Created: 24 Nov 2023 1:56:22pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ADSRComponent.h"
#include "OscComponent.h"

//==============================================================================
/*
*/
class VoiceComponent  : public juce::Component
{
public:
    VoiceComponent(juce::AudioProcessorValueTreeState& apvts);
    ~VoiceComponent() override;

    void setVoice(int voice);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    OscComponent osc;
    AdsrComponent adsr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceComponent)
};
