/*
  ==============================================================================

    OscData.cpp
    Created: 23 Nov 2023 11:12:05pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    adsr.setSampleRate(spec.sampleRate);
    fmOsc.prepare(spec);
    gain.prepare(spec);
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

void OscData::setOscParams(const int octave, const int semi, const float detune, const float gain) {
    oscOctave = octave;
    oscSemi = semi;
    oscDetune = detune;
    oscGain = gain;
    setFrequency(getFrequency());
}

void OscData::setADSR(const float attack, const float decay, const float sustain, const float release) {
    adsr.updateADSR(attack, decay, sustain, release);
}

void OscData::setFMParams(const float depth, const float frequency) {
    fmOsc.setFrequency(frequency);
    fmDepth = depth;
    setFrequency(getFrequency());
}

void OscData::getNextAudioBlock(juce::AudioBuffer<float>& outputBuffer) {
    if (isOn) {
        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch) {
            for (int s = 0; s < outputBuffer.getNumSamples(); ++s) {
                fmMod = fmOsc.processSample(outputBuffer.getSample(ch, s)) * fmDepth;
                outputBuffer.setSample(ch, s, outputBuffer.getSample(ch, s) * oscGain);
            }
        }
        
        juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
        
        // Osc
        process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        // Gain
        gain.setGainLinear(oscGain);
        gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        
        // Adsr
        adsr.applyEnvelopeToBuffer(outputBuffer, 0, outputBuffer.getNumSamples());
    }
}

void OscData::startNote() {
    adsr.noteOn();
}

void OscData::stopNote() {
    adsr.noteOff();
}

