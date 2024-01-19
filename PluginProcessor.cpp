/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Data/SynthData.h"

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
                       ), apvts(*this, nullptr, "Parameters", createParams()), numberOfVoices(numberOfVoices)

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
    
    for (int i = 0; i < NUMBER_OF_VOICES; ++i) {
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

    for (int i = 0; i < NUMBER_OF_VOICES; ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            for (int oscIdx = 0; oscIdx < NUMBER_OF_OSCILLATORS; ++oscIdx) {
                juce::String oscStr = juce::String(oscIdx);
                
                // OSC
                auto &oscWaveType = *apvts.getRawParameterValue("OSCWAVETYPE" + oscStr);
                
                auto &oscOn = *apvts.getRawParameterValue("OSCON" + oscStr);
                auto &oscOctave = *apvts.getRawParameterValue("OSCOCTAVE" + oscStr);
                auto &oscSemi = *apvts.getRawParameterValue("OSCSEMI" + oscStr);
                auto &oscDetune = *apvts.getRawParameterValue("OSCDETUNE" + oscStr);
                auto &oscGain = *apvts.getRawParameterValue("OSCGAIN" + oscStr);

                // ADSR
                auto &ampAttack = *apvts.getRawParameterValue("AMPATTACK" + oscStr);
                auto &ampDecay = *apvts.getRawParameterValue("AMPDECAY" + oscStr);
                auto &ampSustain = *apvts.getRawParameterValue("AMPSUSTAIN" + oscStr);
                auto &ampRelease = *apvts.getRawParameterValue("AMPRELEASE" + oscStr);
                
                // Filter ADSR
                auto &modAttack = *apvts.getRawParameterValue("MODATTACK" + oscStr);
                auto &modDecay = *apvts.getRawParameterValue("MODDECAY" + oscStr);
                auto &modSustain = *apvts.getRawParameterValue("MODSUSTAIN" + oscStr);
                auto &modRelease = *apvts.getRawParameterValue("MODRELEASE" + oscStr);
                
                // FILTER
                auto &filterOn = *apvts.getRawParameterValue("FILTERON" + oscStr);
                auto &filterType = *apvts.getRawParameterValue("FILTERTYPE" + oscStr);
                auto &filterCutoff = *apvts.getRawParameterValue("FILTERCUTOFF" + oscStr);
                auto &filterCutoffLfo = *apvts.getRawParameterValue("FILTERCUTOFFLFO" + oscStr);
                auto &filterRes = *apvts.getRawParameterValue("FILTERRES" + oscStr);
                auto &filterResLfo = *apvts.getRawParameterValue("FILTERRESLFO" + oscStr);
                
                // FM
                auto &lfoWaveType = *apvts.getRawParameterValue("LFOWAVETYPE" + oscStr);
                auto &lfoFreq = *apvts.getRawParameterValue("LFOFREQ" + oscStr);
                
                auto &osc = voice->getOscillator(oscIdx);
                
                osc.setWaveType(oscWaveType);
                osc.setOscOn(oscOn.load());
                osc.setOscParams(oscOctave.load(), oscSemi.load(), oscDetune.load(), oscGain.load());
                osc.setAmpAdsr(ampAttack.load(), ampDecay.load(), ampSustain.load(), ampRelease.load());
                osc.setFilter(filterType.load(), filterCutoff.load(), filterCutoffLfo.load(), filterRes.load(), filterResLfo.load());
                osc.setFilterOn(filterOn.load());
                osc.setModAdsr(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());
                osc.setLfoParams(lfoFreq.load());
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
    return new SynthTalkAudioProcessor(NUMBER_OF_VOICES);
}

// Value Tree

juce::AudioProcessorValueTreeState::ParameterLayout SynthTalkAudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    int paramId = 0;
    
    for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; ++osc) {
        juce::String oscStr = juce::String(osc);

        auto addParam = [&](const juce::String& name, const juce::String& label, const juce::NormalisableRange<float>& range, float defaultValue = 0.0f) {
            ++paramId;
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(name + oscStr, paramId),
                label, range, defaultValue));
            ++paramId;
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(name + "LFO" + oscStr, paramId),
                label, juce::NormalisableRange<float>{ 0.0f, 100.0f, 0.1f }, 0.0f));
        };

        // OSC Select
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID("OSCWAVETYPE" + oscStr, paramId),
            "Osc Wave Type",
            juce::StringArray { "Sine", "Saw", "Square" }, 0, ""));
        
        // OSC
        params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("OSCON" + oscStr, ++paramId), "Osc on", false));
        
        addParam("OSCOCTAVE", "Octave", { -4.0f, 4.0f, 1.0f }, 0.0f);  // Octave can be from -4 to 4
        addParam("OSCSEMI", "Semi", { -12.0f, 12.0f, 1.0f }, 0.0f);   // Semi notes away
        addParam("OSCDETUNE", "Detune", { -100.0f, 100.0f, 1.0f }, 0.0f);  // Detune in cents
        addParam("OSCGAIN", "Gain", { 0.0f, 1.0f, 0.01f }, 0.5f);

        // LFO
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID("LFOWAVETYPE" + oscStr, paramId),
            "Osc Wave Type",
            juce::StringArray { "Sine", "Saw", "Square" }, 0, ""));
        addParam("LFOFREQ", "LFO Frequency", { 0.1f, 20.0f, 0.01f, 0.6f }, 0.1f);
        // addParam("OSCFMDEPTH", "FM Depth", { 0.0f, 1000.0f, 0.01f, 0.3f }, 50.0f);

        // FILTER
        params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("FILTERON" + oscStr, ++paramId), "Filter on", false));
        
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID("FILTERTYPE" + oscStr, paramId),
            "Filter Type",
            juce::StringArray { "Low-Pass", "Band-Pass", "High-Pass" }, 0, ""));
        addParam("FILTERCUTOFF", "Filter Cutoff", { 20.0f, 20000.0f, 0.1f, 0.6f }, 200.0f);
        addParam("FILTERRES", "Filter Resonance", { 1.0f, 10, 0.1f }, 1.0f);
        
        // ADSR
        addParam("AMPATTACK", "Amp Attack", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.005f);
        addParam("AMPDECAY", "Amp Decay", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.625f);
        addParam("AMPSUSTAIN", "Amp Sustain", { 0.0f, 1.0f, 0.001f}, .5f);
        addParam("AMPRELEASE", "Amp Release", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.500f);
        
        // ADSR
        addParam("MODATTACK", "Mod Attack", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.005f);
        addParam("MODDECAY", "Mod Decay", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.625f);
        addParam("MODSUSTAIN", "Mod Sustain", { 0.0f, 1.0f, 0.001f }, .5f);
        addParam("MODRELEASE", "Mod Release", { 0.005f, 15.0f, 0.001f, 0.2f }, 0.500f);
    }

    return { params.begin(), params.end() };
}
