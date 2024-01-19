/*
  ==============================================================================

    FilterData.h
    Created: 30 Nov 2023 9:26:30pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LfoEnabledValue.h"

class FilterData {
public:
    FilterData(LFOData& lfo);
    void prepare(juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    float processSample(int channel, float inputValue);
    void setFilterParameters(const int filterType, const float cutoffFreq, const float cutoffLfoDepth, const float resonance, const float resonanceLfoDepth);
    void setEnvelope(const float env);
    void reset();
    bool isOn { false };
private:
    LfoEnabledValue cutoff;
    LfoEnabledValue resonance;
    juce::dsp::StateVariableTPTFilter<float> filter;
};
