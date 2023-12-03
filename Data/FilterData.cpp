/*
  ==============================================================================

    FilterData.cpp
    Created: 30 Nov 2023 9:26:30pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepare(juce::dsp::ProcessSpec& spec) {
    filter.prepare(spec);
}

void FilterData::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block { buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float> {block});
}

float FilterData::processSample(int channel, float inputValue) {
    if (this->isOn) {
        return filter.processSample(channel, inputValue);
    } else {
        return inputValue;
    }
}

void FilterData::setFilterParameters(const int filterType, const float cutoffFreq, const float resonance) {
    switch (filterType) {
        case 0:
            filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 2:
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
        default:
            jassertfalse;
            break; 
    }
    
    this->cutoff = cutoffFreq;
    this->resonance = resonance;
    filter.setCutoffFrequency(cutoffFreq);
    filter.setResonance(resonance);
}

void FilterData::setModulator(const float modulator) {
    // std::cout << "MOD: " << modulator << std::endl;
    float modFreq = this->cutoff * modulator;
    modFreq = std::fmin(std::fmax(modFreq, 20.0f), 20000.0f); // 20hz <= f <= 20khz
    filter.setCutoffFrequency(modFreq);
}

void FilterData::reset() {
    
}
