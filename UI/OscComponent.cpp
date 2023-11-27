/*
  ==============================================================================

    OscComponent.cpp
    Created: 23 Nov 2023 11:35:06pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    juce::StringArray choices { "Sin", "Saw", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    makeSliderWithLabel(fmFreqSlider, fmFreqLabel);
    makeSliderWithLabel(fmDepthSlider, fmDepthLabel);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
    // Fill the entire component with black
    g.fillAll(juce::Colours::black);

    // Get the bounds of the component
    juce::Rectangle<int> bounds = getLocalBounds();

    // Define the border thickness
    int borderThickness = 2;

    // Draw a rectangle with a white border
    g.setColour(juce::Colours::white);
    g.drawRect(bounds, borderThickness);
}

void OscComponent::resized()
{
    
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    
    oscWaveSelector.setBounds(0, 0, 90, 20);
    
    fmFreqSlider.setBounds(0, 80, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - 20, fmFreqSlider.getWidth(), 20);
    
    fmDepthSlider.setBounds(fmFreqSlider.getRight(), 80, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - 20, fmDepthSlider.getWidth(), 20);
}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void OscComponent::makeSliderWithLabel(juce::Slider& slider, juce::Label& label) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    slider.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void OscComponent::setVoice(int voice) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    if (oscWaveSelectorAttachment != nullptr)
        oscWaveSelectorAttachment.reset();

    if (fmFreqAttachment != nullptr)
        fmFreqAttachment.reset();

    if (fmDepthAttachment != nullptr)
        fmDepthAttachment.reset();

    juce::String voiceStr = juce::String(voice);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "OSCWAVETYPE" + voiceStr, oscWaveSelector);

    fmFreqAttachment = std::make_unique<SliderAttachment>(apvts, "OSCFMFREQ" + voiceStr, fmFreqSlider);
    fmDepthAttachment = std::make_unique<SliderAttachment>(apvts, "OSCFMDEPTH" + voiceStr, fmDepthSlider);
}
