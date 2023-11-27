/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTalkAudioProcessor::SynthTalkAudioProcessor(int numberOfVoices)
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams(numberOfVoices)), numberOfVoices(numberOfVoices)

#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < numberOfVoices; ++i) {
        synth.addVoice(new SynthVoice());
    }
}

SynthTalkAudioProcessor::~SynthTalkAudioProcessor()
{
}

//==============================================================================
const juce::String SynthTalkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthTalkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthTalkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthTalkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthTalkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthTalkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthTalkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthTalkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthTalkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthTalkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthTalkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice((i)))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SynthTalkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthTalkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthTalkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            juce::String voiceStr = juce::String(i);
            
            auto &attack = *apvts.getRawParameterValue("ATTACK" + voiceStr);
            auto &decay = *apvts.getRawParameterValue("DECAY" + voiceStr);
            auto &sustain = *apvts.getRawParameterValue("SUSTAIN" + voiceStr);
            auto &release = *apvts.getRawParameterValue("RELEASE" + voiceStr);

            auto &oscWaveType = *apvts.getRawParameterValue("OSCWAVETYPE" + voiceStr);

            auto &fmDepth = *apvts.getRawParameterValue("OSCFMDEPTH" + voiceStr);
            auto &fmFreq = *apvts.getRawParameterValue("OSCFMFREQ" + voiceStr);

            // UPDATE HERE
            for (int i = 0; i < 5; ++i) {
                voice->getOscillator(i).setWaveType(oscWaveType);
                voice->getOscillator(i).setFMParams(fmDepth.load(), fmFreq.load());
                voice->update(attack.load(), decay.load(), sustain.load(), release.load());
            }
        }
    }
        
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthTalkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthTalkAudioProcessor::createEditor()
{
    return new SynthTalkAudioProcessorEditor (*this);
}

//==============================================================================
void SynthTalkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthTalkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthTalkAudioProcessor(5);
}

// Value Tree

juce::AudioProcessorValueTreeState::ParameterLayout SynthTalkAudioProcessor::createParams(int numberOfVoices) {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    int paramId = 0;
    
    for (int voice = 0; voice < numberOfVoices; ++voice) {
        juce::String voiceStr = juce::String(voice);

        auto addParam = [&](const juce::String& name, const juce::String& label, const juce::NormalisableRange<float>& range, float defaultValue = 0.0f) {
            ++paramId;
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(name + voiceStr, paramId),
                label, range, defaultValue));
        };

        // OSC Select
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID("OSCWAVETYPE" + voiceStr, paramId),
            "Osc Wave Type",
            juce::StringArray { "Sine", "Saw", "Square" }, 0, ""));

        // FM
        addParam("OSCFMFREQ", "FM Frequency", { 0.0f, 1000.0f, 0.01f, 0.3f }, 5.0f);
        addParam("OSCFMDEPTH", "FM Depth", { 0.0f, 1000.0f, 0.01f, 0.3f }, 50.0f);

        // ADSR
        addParam("ATTACK", "Attack", { 0.1f, 1.0f, 0.1f }, 0.1f);
        addParam("DECAY", "Decay", { 0.1f, 1.0f, 0.1f }, 0.1f);
        addParam("SUSTAIN", "Sustain", { 0.1f, 1.0f, 0.1f }, 1.0f);
        addParam("RELEASE", "Release", { 0.1f, 3.0f, 0.1f }, 0.4f);

        // GAIN
        addParam("GAIN", "Gain", { 0.0f, 1.0f, 0.01f }, 0.5f);
    }

    return { params.begin(), params.end() };
}
