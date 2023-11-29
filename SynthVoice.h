/*
  ==============================================================================

    SynthVoice.h
    Created: 22 Nov 2023 12:01:29pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/ADSRData.h"
#include "Data/OscData.h"
#include "Data/SynthData.h"


class SynthVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
    OscData& getOscillator(int osc) { return oscillators[osc]; };
    
private:
    juce::AudioBuffer<float> synthBuffer;
    
    OscData oscillators[NUMBER_OF_OSCILLATORS];
    
    juce::dsp::Gain<float> gain;
    bool isPrepared { false };
};



