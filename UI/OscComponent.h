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
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    
    juce::Slider oscOctaveSlider;
    juce::Slider oscSemiSlider;
    juce::Slider oscDetuneSlider;
    
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscOctaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscSemiAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscDetuneAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthAttachment;
    
    juce::Label oscOctaveLabel { "Octave", "Octave" };
    juce::Label oscSemiLabel { "Semi", "Semi" };
    juce::Label oscDetuneLabel { "Detune", "Detune" };
    
    juce::Label fmFreqLabel { "FM Freq", "FM Freq" };
    juce::Label fmDepthLabel { "FM Depth", "FM Depth" };
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    void makeSliderWithLabel (juce::Slider& slider, juce::Label& label);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
