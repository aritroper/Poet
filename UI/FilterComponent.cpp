/*
  ==============================================================================

    FilterComponent.cpp
    Created: 2 Dec 2023 4:36:19pm
    Author:  Ari Troper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts), filterOnButton("Off") {
    juce::StringArray choices { "Low-Pass", "High-Pass", "Band-Pass" };
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);
    
    filterOnButton.setClickingTogglesState(true);
    filterOnButton.onClick = [this]()
    {
        if (filterOnButton.getToggleState())
        {
            filterOnButton.setButtonText("On");
            // Additional logic for when the button is toggled on
        }
        else
        {
            filterOnButton.setButtonText("Off");
            // Additional logic for when the button is toggled off
        }
    };
    addAndMakeVisible(filterOnButton);
    
    makeSliderWithLabel(filterCutoffSlider, filterFreqLabel);
    makeSliderWithLabel(filterResonanceSlider, filterResLabel);
}

FilterComponent::~FilterComponent() {
    
}

void FilterComponent::setOsc(int osc) {
    if (filterTypeSelectorAttachment != nullptr)
        filterTypeSelectorAttachment.reset();

    if (filterAttachment != nullptr)
        filterAttachment.reset();
    
    if (resonanceAttachment != nullptr)
        resonanceAttachment.reset();
    
    if (filterOnAttachment != nullptr)
        filterOnAttachment.reset();

    juce::String oscStr = juce::String(osc);
    
    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "FILTERTYPE" + oscStr, filterTypeSelector);
    
    filterOnAttachment =  std::make_unique<ButtonAttachment>(apvts, "FILTERON" + oscStr, filterOnButton);

    filterAttachment = std::make_unique<SliderAttachment>(apvts, "FILTERCUTOFF" + oscStr, filterCutoffSlider);
    resonanceAttachment = std::make_unique<SliderAttachment>(apvts, "FILTERRES" + oscStr, filterResonanceSlider);
}

void FilterComponent::paint(juce::Graphics& g){
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

void FilterComponent::resized() {
    const auto bounds = getLocalBounds();
    const auto sliderWidth = 50;
    const auto sliderHeight = 80;
    const auto padding = 10;
    const auto startX = (bounds.getWidth() - (sliderWidth*2 + padding)) / 2;
    
    filterTypeSelector.setBounds(0, 0, 120, 20);
    
    filterOnButton.setBounds(filterTypeSelector.getRight() + 20, 0, 60, 20);
    
    filterCutoffSlider.setBounds(startX, 80, sliderWidth, sliderHeight);
    filterFreqLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getY() - 20, filterCutoffSlider.getWidth(), 20);
    
    filterResonanceSlider.setBounds(filterFreqLabel.getRight() + padding, 80, sliderWidth, sliderHeight);
    filterResLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getY() - 20, filterResonanceSlider.getWidth(), 20);
}

void FilterComponent::makeSliderWithLabel(juce::Slider &slider, juce::Label &label) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    slider.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
