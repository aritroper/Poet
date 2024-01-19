/*
  ==============================================================================

    OscData.h
    Created: 23 Nov 2023 11:12:05pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ADSRData.h"
#include "FilterData.h"
#include "LFOData.h"

class OscData : public juce::dsp::Oscillator<float> {
public:
    OscData();
    float getFrequency();
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int waveType);
    void setWaveFrequency (const int midiNoteNumber);
    void getNextAudioBlock(juce::AudioBuffer<float>& outputBuffer);
    void setOscOn(const bool isOn) { this->isOn = isOn; };
    void setOscParams(const int octave, const int semi, const float detune, const float gain);
    void setAmpAdsr(const float attack, const float decay, const float sustain, const float release);
    void setFilterOn(const bool isOn) { this->filter.isOn = isOn; };
    void setFilter(const int filterType, const float cutoffFreq, const float cutoffLfoDepth, const float resonance, const float resonanceLfoDepth);
    void setLfoParams(const float frequency);
    void setModAdsr(const float attack, const float decay, const float sustain, const float release);
    void setFMParams(const float depth, const float frequency);
    AdsrData& getAmpAdsr() { return ampAdsr; };
    AdsrData& getModAdsr() { return modAdsr; };
    bool isOn { false };
private:
    juce::dsp::Gain<float> gain;
    AdsrData ampAdsr;
    AdsrData modAdsr;
    LFOData lfo;
    FilterData filter;
    int oscOctave { 0 };
    int oscSemi { 0 };
    float oscDetune { 0.0f };
    float oscGain { 0.5f };
    int lastMidiNote { 0 };
};
