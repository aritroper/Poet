/*
  ==============================================================================

    WaveformView.cpp
    Created: 24 Nov 2023 1:26:09pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformView.h"

//==============================================================================
WaveformView::WaveformView()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
}

WaveformView::~WaveformView()
{
}

void WaveformView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Draw the waveform
    g.setColour(juce::Colours::white);

    int numChannels = waveformBuffer.getNumChannels();
    int width = getWidth();
    int height = getHeight();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        juce::Path waveformPath;
        waveformPath.startNewSubPath(0, height / 2);

        for (int sample = 0; sample < waveformBuffer.getNumSamples(); ++sample)
        {
            float x = sample / static_cast<float>(waveformBuffer.getNumSamples() - 1) * width;
            float y = waveformBuffer.getSample(channel, sample) * height / 2 + height / 2;
            waveformPath.lineTo(x, y);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
    }
}

void WaveformView::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformView::setWaveformBuffer(juce::AudioBuffer<float>& newBuffer)
{
    waveformBuffer = newBuffer;
    repaint();
}
