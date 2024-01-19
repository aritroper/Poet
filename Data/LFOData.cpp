/*
  ==============================================================================

    LFOData.cpp
    Created: 3 Dec 2023 1:13:48pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "LFOData.h"

void LFOData::setWaveType(const int waveType) {
    switch (waveType) {
        case 0:
            this->osc = juce::dsp::Oscillator<float>([](float x) { return std::sin(x); });
            break;
        case 1:
            this->osc = juce::dsp::Oscillator<float>([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            this->osc = juce::dsp::Oscillator<float>([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        default:
            jassertfalse;
            break;
    }
}

void LFOData::setLfoParams(float frequency) {
    this->frequency = frequency;
    osc.setFrequency(frequency);
}

void LFOData::prepare(juce::dsp::ProcessSpec& spec) {
    osc.prepare(spec);
}

void LFOData::processSample(float input) {
    this->mod = osc.processSample(input);
}
