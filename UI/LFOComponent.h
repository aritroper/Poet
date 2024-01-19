/*
  ==============================================================================

    LFOComponent.h
    Created: 3 Dec 2023 1:13:40pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOComponent  : public juce::Component
{
public:
    LFOComponent(juce::AudioProcessorValueTreeState& apvts);
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setOsc(int osc);

private:
    juce::AudioProcessorValueTreeState& apvts;
    
    juce::ComboBox oscWaveSelector;
    juce::Slider frequencySlider;
    juce::Slider depthSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    juce::Label frequencyLabel { "Frequency", "Freq" };
    juce::Label depthLabel { "Depth", "Depth" };
    
    std::unique_ptr<SliderAttachment> frequencyAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    
    void makeSliderWithLabel(juce::Slider &slider, juce::Label &label);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
