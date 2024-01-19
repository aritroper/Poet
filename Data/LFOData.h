/*
  ==============================================================================

    LFOData.h
    Created: 3 Dec 2023 1:13:48pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFOData {
public:
    void setWaveType(const int waveType);
    void setLfoParams(float frequency);
    void prepare(juce::dsp::ProcessSpec& spec);
    void processSample(float input);
    float mod { 0.0f };
private:
    juce::dsp::Oscillator<float> osc { [](float x) { return std::sin(x); }};
    float frequency { 0.0f };
};
