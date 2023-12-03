/*
  ==============================================================================

    FilterData.h
    Created: 30 Nov 2023 9:26:30pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData {
public:
    void prepare(juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    float processSample(int channel, float inputValue);
    void setFilterParameters(const int filterType, const float cutoffFreq, const float resonance);
    void setModulator(const float modulator);
    void reset();
    bool isOn { false };
private:
    float cutoff;
    float resonance;
    juce::dsp::StateVariableTPTFilter<float> filter;
};
