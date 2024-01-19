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
#include "FilterComponent.h"
#include "LFOComponent.h"

//==============================================================================
/*
*/
class VoiceComponent  : public juce::Component
{
public:
    VoiceComponent(juce::AudioProcessorValueTreeState& apvts);
    ~VoiceComponent() override;

    void setOsc(int osc);
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void makeOscButtons();
    void loadOsc(int osc);

private:
    juce::OwnedArray<juce::TextButton> oscButtons;
    
    OscComponent osc;
    AdsrComponent ampEnvelope;
    FilterComponent filter;
    AdsrComponent modEnvelope;
    LFOComponent lfo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceComponent)
};
