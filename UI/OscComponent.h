/*
  ==============================================================================

    OscComponent.h
    Created: 23 Nov 2023 11:35:06pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts);
    ~OscComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void setOsc(int osc);

private:
    juce::AudioProcessorValueTreeState& apvts;
    
    juce::ComboBox oscWaveSelector;
    juce::TextButton oscOnButton;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    
    juce::Slider oscGainSlider;
    juce::Slider oscOctaveSlider;
    juce::Slider oscSemiSlider;
    juce::Slider oscDetuneSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    std::unique_ptr<ButtonAttachment> oscOnAttachment;
    
    std::unique_ptr<SliderAttachment> oscGainAttachment;
    std::unique_ptr<SliderAttachment> oscOctaveAttachment;
    std::unique_ptr<SliderAttachment> oscSemiAttachment;
    std::unique_ptr<SliderAttachment> oscDetuneAttachment;
    
    juce::Label oscGainLabel { "Gain", "Gain" };
    juce::Label oscOctaveLabel { "Octave", "Octave" };
    juce::Label oscSemiLabel { "Semi", "Semi" };
    juce::Label oscDetuneLabel { "Detune", "Detune" };
    
    void makeSliderWithLabel (juce::Slider& slider, juce::Label& label);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
