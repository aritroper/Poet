/*
  ==============================================================================

    OscData.h
    Created: 23 Nov 2023 11:12:05pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float> {
public:
    float getFrequency();
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int waveType);
    void setWaveFrequency (const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void setOscParams(const int octave, const int semi, const float detune);
    void setFMParams(const float depth, const float frequency);
private:
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin(x); }};
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int oscOctave { 0 };
    int oscSemi { 0 };
    float oscDetune { 0.0f };
    int lastMidiNote { 0 };
};
