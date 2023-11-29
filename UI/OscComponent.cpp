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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts), oscOnButton("On")
{
    juce::StringArray choices { "Sin", "Saw", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    
    oscOnButton.setClickingTogglesState(true);
    oscOnButton.onClick = [this]()
    {
        if (oscOnButton.getToggleState())
        {
            oscOnButton.setButtonText("On");
            // Additional logic for when the button is toggled on
        }
        else
        {
            oscOnButton.setButtonText("Off");
            // Additional logic for when the button is toggled off
        }
    };
    addAndMakeVisible(oscOnButton);
    
    makeSliderWithLabel(oscOctaveSlider, oscOctaveLabel);
    makeSliderWithLabel(oscSemiSlider, oscSemiLabel);
    makeSliderWithLabel(oscDetuneSlider, oscDetuneLabel);
    makeSliderWithLabel(oscGainSlider, oscGainLabel);
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
    
    const auto sliderWidth = 50;
    const auto sliderHeight = 80;
    
    oscWaveSelector.setBounds(0, 0, 90, 20);
    
    oscOnButton.setBounds(oscWaveSelector.getRight() + 20, 0, 90, 20);
    
    oscOctaveSlider.setBounds(0, 80, sliderWidth, sliderHeight);
    oscOctaveLabel.setBounds(oscOctaveSlider.getX(), oscOctaveSlider.getY() - 20, oscOctaveSlider.getWidth(), 20);
    
    oscSemiSlider.setBounds(oscOctaveSlider.getRight(), 80, sliderWidth, sliderHeight);
    oscSemiLabel.setBounds(oscSemiSlider.getX(), oscSemiSlider.getY() - 20, oscSemiSlider.getWidth(), 20);
    
    oscDetuneSlider.setBounds(oscSemiSlider.getRight(), 80, sliderWidth, sliderHeight);
    oscDetuneLabel.setBounds(oscDetuneSlider.getX(), oscDetuneSlider.getY() - 20, oscDetuneSlider.getWidth(), 20);
    
    oscGainSlider.setBounds(oscDetuneSlider.getRight(), 80, sliderWidth, sliderHeight);
    oscGainLabel.setBounds(oscGainSlider.getX(), oscGainSlider.getY() - 20, oscGainSlider.getWidth(), 20);
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

void OscComponent::setOsc(int osc) {
    if (oscWaveSelectorAttachment != nullptr)
        oscWaveSelectorAttachment.reset();

    if (fmFreqAttachment != nullptr)
        fmFreqAttachment.reset();

    if (fmDepthAttachment != nullptr)
        fmDepthAttachment.reset();
    
    if (oscOnAttachment != nullptr)
        oscOnAttachment.reset();
    
    if (oscGainAttachment != nullptr)
        oscGainAttachment.reset();
    
    if (oscOctaveAttachment != nullptr)
        oscOctaveAttachment.reset();
    
    if (oscSemiAttachment != nullptr)
        oscSemiAttachment.reset();
    
    if (oscDetuneAttachment != nullptr)
        oscDetuneAttachment.reset();

    juce::String oscStr = juce::String(osc);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "OSCWAVETYPE" + oscStr, oscWaveSelector);

    fmFreqAttachment = std::make_unique<SliderAttachment>(apvts, "OSCFMFREQ" + oscStr, fmFreqSlider);
    fmDepthAttachment = std::make_unique<SliderAttachment>(apvts, "OSCFMDEPTH" + oscStr, fmDepthSlider);
    
    oscOctaveAttachment = std::make_unique<SliderAttachment>(apvts, "OSCOCTAVE" + oscStr, oscOctaveSlider);
    oscSemiAttachment = std::make_unique<SliderAttachment>(apvts, "OSCSEMI" + oscStr, oscSemiSlider);
    oscDetuneAttachment = std::make_unique<SliderAttachment>(apvts, "OSCDETUNE" + oscStr, oscDetuneSlider);
    oscGainAttachment = std::make_unique<SliderAttachment>(apvts, "OSCGAIN" + oscStr, oscGainSlider);
    
    oscOnAttachment = std::make_unique<ButtonAttachment>(apvts, "OSCON" + oscStr, oscOnButton);
}
