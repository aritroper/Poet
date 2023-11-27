/*
  ==============================================================================

    WaveformView.h
    Created: 24 Nov 2023 1:26:09pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformView  : public juce::Component
{
public:
    WaveformView();
    ~WaveformView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setWaveformBuffer(juce::AudioBuffer<float>& newBuffer);

private:
    juce::AudioBuffer<float> waveformBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformView)
};
