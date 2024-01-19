/*
  ==============================================================================

    OscData.cpp
    Created: 23 Nov 2023 11:12:05pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "OscData.h"

OscData::OscData() : filter(lfo) {
    // Constructor body
    // Initialize other member variables if needed
}


void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    ampAdsr.setSampleRate(spec.sampleRate);
    modAdsr.setSampleRate(spec.sampleRate);
    lfo.prepare(spec);
    gain.prepare(spec);
    filter.prepare(spec);
    prepare(spec);
}

float OscData::getFrequency() {
    float frequencyMultiplier = std::pow(2.0f, static_cast<float>(oscOctave) + oscSemi / 12.0f);
    float detuneFactor = std::pow(2.0f, oscDetune / 1200.0f);
    float freq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) * frequencyMultiplier * detuneFactor;
    freq += std::abs(std::min(0.0f, freq));
    return freq;
}

void OscData::setWaveType(const int waveType) {
    switch (waveType) {
        case 0:
            initialise([](float x) { return std::sin(x); });
            break;
        case 1:
            initialise([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        default:
            jassertfalse;
            break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber) {
    lastMidiNote = midiNoteNumber;
    setFrequency(getFrequency());
}

void OscData::setOscParams(const int octave, const int semi, const float detune, const float gain) {
    oscOctave = octave;
    oscSemi = semi;
    oscDetune = detune;
    oscGain = gain;
    setFrequency(getFrequency());
}

void OscData::setAmpAdsr(const float attack, const float decay, const float sustain, const float release) {
    ampAdsr.setAdsrParameters(attack, decay, sustain, release);
}

void OscData::setModAdsr(const float attack, const float decay, const float sustain, const float release) {
    modAdsr.setAdsrParameters(attack, decay, sustain, release);
}

void OscData::setFilter(const int filterType, const float cutoffFreq, const float cutoffLfoDepth, const float resonance, const float resonanceLfoDepth) {
    filter.setFilterParameters(filterType, cutoffFreq, cutoffLfoDepth, resonance, resonanceLfoDepth);
}

void OscData::setLfoParams(const float frequency) {
    lfo.setLfoParams(frequency);
}

void OscData::getNextAudioBlock(juce::AudioBuffer<float>& outputBuffer) {
    jassert(isOn);
    
    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    
    // Osc
    process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    // Gain
    gain.setGainLinear(oscGain);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    // Amp adsr
    ampAdsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
    
    for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch) {
        for (int s = 0; s < outputBuffer.getNumSamples(); ++s) {
            // FM
            lfo.processSample(outputBuffer.getSample(ch, s));
            
            // Filter + ModAdsr
            filter.setEnvelope(modAdsr.getNextSample());
            const float filterSample = filter.processSample(ch, outputBuffer.getSample(ch, s));
            outputBuffer.setSample(ch, s, filterSample);
        }
    }
}

