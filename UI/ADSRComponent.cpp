/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 23 Nov 2023 10:39:45pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId) : apvts(apvts)
{
    this->attackId = attackId;
    this->decayId = decayId;
    this->sustainId = sustainId;
    this->releaseId = releaseId;
    
    makeSliderWithLabel(attackSlider, attackLabel);
    makeSliderWithLabel(decaySlider, decayLabel);
    makeSliderWithLabel(sustainSlider, sustainLabel);
    makeSliderWithLabel(releaseSlider, releaseLabel);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::setOsc(int osc) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    if (attackAttachment != nullptr)
        attackAttachment.reset();

    if (decayAttachment != nullptr)
        decayAttachment.reset();

    if (sustainAttachment != nullptr)
        sustainAttachment.reset();

    if (releaseAttachment != nullptr)
        releaseAttachment.reset();

    juce::String oscStr = juce::String(osc);

    attackAttachment = std::make_unique<SliderAttachment>(apvts, attackId + oscStr, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, decayId + oscStr, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, sustainId + oscStr, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, releaseId + oscStr, releaseSlider);
}

void AdsrComponent::paint (juce::Graphics& g)
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

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds();
    const auto padding = 10;
    const auto sliderWidth = ((bounds.getWidth() - 2*padding) / 4) - padding;
    const auto sliderHeight = bounds.getHeight() - 45;
    const auto sliderStartX = padding + 5;
    const auto sliderStartY = bounds.getHeight() / 2 - ((sliderHeight - 25)  / 2);
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto labelStart = sliderStartY - labelYOffset;
    
    attackSlider.setBounds (sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    attackLabel.setBounds (attackSlider.getX(), labelStart, sliderWidth, labelHeight);
    
    decaySlider.setBounds (attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    decayLabel.setBounds (decaySlider.getX(), labelStart, sliderWidth, labelHeight);
    
    sustainSlider.setBounds (decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainLabel.setBounds (sustainSlider.getX(), labelStart, sliderWidth, labelHeight);
    
    releaseSlider.setBounds (sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseLabel.setBounds (releaseSlider.getX(), labelStart, sliderWidth, labelHeight);
}

void AdsrComponent::makeSliderWithLabel (juce::Slider& slider, juce::Label& label) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    slider.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
