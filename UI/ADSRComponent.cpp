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
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    makeSlider(attackSlider);
    makeSlider(decaySlider);
    makeSlider(sustainSlider);
    makeSlider(releaseSlider);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::setVoice(int voice) {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    if (attackAttachment != nullptr)
        attackAttachment.reset();

    if (decayAttachment != nullptr)
        decayAttachment.reset();

    if (sustainAttachment != nullptr)
        sustainAttachment.reset();

    if (releaseAttachment != nullptr)
        releaseAttachment.reset();

    juce::String voiceStr = juce::String(voice);

    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK" + voiceStr, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY" + voiceStr, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN" + voiceStr, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE" + voiceStr, releaseSlider);
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
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight();
    const auto sliderStartX = 0;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 2);
    
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

}

void AdsrComponent::makeSlider(juce::Slider& slider) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
