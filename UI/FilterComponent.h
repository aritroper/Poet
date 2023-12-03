/*
  ==============================================================================

    FilterComponent.h
    Created: 2 Dec 2023 4:36:19pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts);
    ~FilterComponent() override;

    void setOsc(int osc);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& apvts;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    juce::ComboBox filterTypeSelector { "Filter Type" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    
    juce::Label filterSelectorLabel { "Filter Type", "Filter Type" };
    juce::Label filterFreqLabel { "Cutoff", "Cutoff" };
    juce::Label filterResLabel { "Res", "Res" };
    
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    
    std::unique_ptr<SliderAttachment> filterAttachment;
    std::unique_ptr<SliderAttachment> resonanceAttachment;

    void makeSliderWithLabel (juce::Slider& slider, juce::Label& label);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
