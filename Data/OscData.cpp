/*
  ==============================================================================

    OscData.cpp
    Created: 23 Nov 2023 11:12:05pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    fmOsc.prepare(spec);
    prepare(spec);
}

float OscData::getFrequency() {
    float frequencyMultiplier = std::pow(2.0f, static_cast<float>(oscOctave) + oscSemi / 12.0f);
    float detuneFactor = std::pow(2.0f, oscDetune / 1200.0f);
    float freq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) * frequencyMultiplier * detuneFactor + fmMod;
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

void OscData::setOscParams(const int octave, const int semi, const float detune) {
    oscOctave = octave;
    oscSemi = semi;
    oscDetune = detune;
    setFrequency(getFrequency());
}

void OscData::setFMParams(const float depth, const float frequency) {
    fmOsc.setFrequency(frequency);
    fmDepth = depth;
    setFrequency(getFrequency());
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    for (int ch = 0; ch < block.getNumChannels(); ++ch) {
        for (int s = 0; s < block.getNumSamples(); ++s) {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
    process(juce::dsp::ProcessContextReplacing<float> (block));
}

