/*
  ==============================================================================

    LFOComponent.cpp
    Created: 3 Dec 2023 1:13:40pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOComponent.h"

//==============================================================================
LFOComponent::LFOComponent(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    juce::StringArray choices { "Sin", "Saw", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    makeSliderWithLabel(frequencySlider, frequencyLabel);
}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::paint (juce::Graphics& g)
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

void LFOComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    const auto bounds = getLocalBounds();
    const auto sliderWidth = 50;
    const auto sliderHeight = 80;
    const auto padding = 10;
    const auto startX = (bounds.getWidth() - (sliderWidth + padding)) / 2;
    
    oscWaveSelector.setBounds(0, 0, 90, 20);
    
    frequencySlider.setBounds(startX, 80, sliderWidth, sliderHeight);
    frequencyLabel.setBounds(frequencySlider.getX(), frequencySlider.getY() - 20, frequencySlider.getWidth(), 20);
}

void LFOComponent::setOsc(int osc) {
    if (oscWaveSelectorAttachment != nullptr)
        oscWaveSelectorAttachment.reset();
    
    if (frequencyAttachment != nullptr)
        frequencyAttachment.reset();

    juce::String oscStr = juce::String(osc);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "LFOWAVETYPE" + oscStr, oscWaveSelector);
    
    frequencyAttachment = std::make_unique<SliderAttachment>(apvts, "LFOFREQ" + oscStr, frequencySlider);
}

void LFOComponent::makeSliderWithLabel(juce::Slider &slider, juce::Label &label) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    slider.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
