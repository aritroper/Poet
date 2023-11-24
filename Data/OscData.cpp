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
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::setFMParams(const float depth, const float frequency) {
    fmOsc.setFrequency(frequency);
    fmDepth = depth;
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    for (int ch = 0; ch < block.getNumChannels(); ++ch) {
        for (int s = 0; s < block.getNumSamples(); ++s) {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
    process(juce::dsp::ProcessContextReplacing<float> (block));
}

