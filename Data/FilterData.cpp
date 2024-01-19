/*
  ==============================================================================

    FilterData.cpp
    Created: 30 Nov 2023 9:26:30pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "FilterData.h"

FilterData::FilterData(LFOData& lfo) : cutoff(lfo, 200, 200, 20000), resonance(lfo, 1, 1, 10) {};

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

void FilterData::setFilterParameters(const int filterType, const float cutoffFreq, const float cutoffLfoDepth, const float resonance, const float resonanceLfoDepth) {
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
    
    this->cutoff.setValue(cutoffFreq);
    this->cutoff.setDepth(cutoffLfoDepth);
    this->resonance.setValue(resonance);
    this->resonance.setDepth(resonanceLfoDepth);
    filter.setCutoffFrequency(this->cutoff.getLfodValue());
    filter.setResonance(this->resonance.getLfodValue());
}

void FilterData::setEnvelope(const float env) {
    float modFreq = this->cutoff.getLfodValue() * env;
    filter.setCutoffFrequency(modFreq);
}

void FilterData::reset() {
    
}
