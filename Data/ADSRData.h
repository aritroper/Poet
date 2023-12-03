/*
  ==============================================================================

    ADSRData.h
    Created: 23 Nov 2023 10:40:20pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AdsrData : public juce::ADSR {
public:
    void setAdsrParameters (const float attack, const float decay, const float sustain, const float release);
private:
    juce::ADSR::Parameters adsrParams;
};
