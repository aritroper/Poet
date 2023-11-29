/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 Nov 2023 12:01:29pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "SynthVoice.h"
#include "Data/SynthData.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; ++osc) {
        oscillators[osc].setWaveFrequency(midiNoteNumber);
        oscillators[osc].startNote();
    }
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
    for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; ++osc) {
        oscillators[osc].stopNote();
    }
    
    // || !adsr.isActive()
    if (!allowTailOff) {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {
    
}


void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    for (int i = 0; i < NUMBER_OF_OSCILLATORS; ++i) {
        oscillators[i].prepareToPlay(spec);
        oscillators[i].setFrequency(440.0f); // You can set different frequencies here if needed
    }

    gain.prepare(spec);
    gain.setGainLinear(0.3f);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    if (!isVoiceActive()) {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    
    
    for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; ++osc) {
        synthBuffer.clear();
        juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
        oscillators[osc].getNextAudioBlock(synthBuffer);
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
            outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        }
    }
}
