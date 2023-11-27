/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 Nov 2023 12:01:29pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    osc.setWaveFrequency(midiNoteNumber);
    osc2.setWaveFrequency(midiNoteNumber + 5);
    adsr.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {
    
}


void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc.prepareToPlay(spec);
    osc2.prepareToPlay(spec);
    gain.prepare(spec);
    
    osc.setFrequency(440.0f);
    osc2.setFrequency(440.0f);
    gain.setGainLinear(0.3f);
    
    isPrepared = true;
}

void SynthVoice::update (const float attack, const float decay, const float sustain, const float release) {
    adsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);

    if (!isVoiceActive()) {
        return;
    }

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // Process the first oscillator
    juce::dsp::AudioBlock<float> audioBlock1{ synthBuffer };
    osc.getNextAudioBlock(audioBlock1);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock1));

    // Process the second oscillator
    juce::dsp::AudioBlock<float> audioBlock2{ synthBuffer };
    osc2.getNextAudioBlock(audioBlock2);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock2));

    // Apply ADSR envelope to the combined buffer
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}
