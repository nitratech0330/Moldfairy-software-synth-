#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 1

#include "JuceHeader.h"

#include "PluginProcessor.h"

#include "PluginEditor.h"

#include <string.h>

#include <algorithm>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       STATES (*this, nullptr, "NITRATECH", paramlayout())
{
    STATES.addParameterListener ("CUTO", this);
    STATES.addParameterListener ("RESO", this);
    STATES.addParameterListener ("REMO", this);
    STATES.addParameterListener ("CUMO", this);
    STATES.addParameterListener ("ATTA", this);
    STATES.addParameterListener ("DECA", this);
    STATES.addParameterListener ("SUST", this);
    STATES.addParameterListener ("RELE", this);
    STATES.addParameterListener ("FIAT", this);
    STATES.addParameterListener ("FIDE", this);
    STATES.addParameterListener ("FISU", this);
    STATES.addParameterListener ("FIRE", this);
    STATES.addParameterListener ("SHAP", this);
    STATES.addParameterListener ("MOAP", this);
}
AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    STATES.removeParameterListener ("CUTO", this);
    STATES.removeParameterListener ("RESO", this);
    STATES.removeParameterListener ("REMO", this);
    STATES.removeParameterListener ("CUMO", this);
    STATES.removeParameterListener ("ATTA", this);
    STATES.removeParameterListener ("DECA", this);
    STATES.removeParameterListener ("SUST", this);
    STATES.removeParameterListener ("RELE", this);
    STATES.removeParameterListener ("FIAT", this);
    STATES.removeParameterListener ("FIDE", this);
    STATES.removeParameterListener ("FISU", this);
    STATES.removeParameterListener ("FIRE", this);
    STATES.removeParameterListener ("SHAP", this);
    STATES.removeParameterListener ("MOAP", this);

    const juce::ScopedLock sl (getCallbackLock());
    Synth.clearVoices();
    Synth.clearSounds();
}
//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return "Moldfairy2";
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================


void AudioPluginAudioProcessor::releaseResources()
{
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}



//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //juce::ignoreUnused (destData);
    juce::XmlElement xml ("states");
    auto State = STATES.copyState();
    std::unique_ptr<juce::XmlElement>apvtsXml (State.createXml());
    xml.addChildElement (apvtsXml.release());
    copyXmlToBinary (xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //juce::ignoreUnused (data, sizeInBytes);
    std::unique_ptr<juce::XmlElement> xmlStates (getXmlFromBinary (data, sizeInBytes));
    if (xmlStates != nullptr){
        if (xmlStates->hasTagName("states")){
            auto apS = xmlStates->getChildByName(STATES.state.getType().toString());
            if (apS != nullptr){
                STATES.replaceState(juce::ValueTree::fromXml(*apS));
                updateHostDisplay();
            }
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::paramlayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout leyout;

    

    juce::StringArray SAw;
    SAw.add("SuperSaw off");
    SAw.add("SuperSaw 0n");

    juce::StringArray numv;
    numv.add ("single");
    numv.add ("2");
    numv.add ("3");
    numv.add ("4");
    numv.add ("5");
    numv.add ("6");
    numv.add ("SuperSaw");

    juce::StringArray Waveforms;
    Waveforms.add ("SAW");
    Waveforms.add ("SINE");
    Waveforms.add ("FMwave");
    Waveforms.add ("RECTANGLE");

    juce::StringArray Ftype;
    Ftype.add("Lowpass");
    Ftype.add("Highpass");

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("CUTO", 1),
        "Cutoff", 0.0f, 1.0f, 0.99f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("RESO", 1),
        "Emphasis", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("REMO", 1),
        "Emphasis MOD", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("CUMO", 1),
        "Cutoff MOD", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("ATTA", 1),
        "Attack", 0.0f, 0.4f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("DECA", 1),
        "Decay", 0.0f, 0.4f, 0.2f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("SUST", 1),
        "Sustin", 0.001f, 1.0f, 0.2f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("RELE", 1),
        "Release", 0.0f, 2.0f, 0.1f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FIAT", 1),
        "Filter Attack", 0.0f, 0.4f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FIDE", 1),
        "Filter Decay", 0.0f, 0.4f, 0.2f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FISU", 1),
        "Filter Sustin", 0.001f, 1.0f, 0.2f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FIRE", 1),
        "Filter Release", 0.0f, 2.0f, 0.1f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FINE1", 1),
        "OSC 1 Fine Tune", -0.09f, 0.09f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FINE2", 1),
        "OSC 2 Fine Tune", -0.09f, 0.09f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FINE3", 1),
        "OSC 3 Fine Tune", -0.09f, 0.09f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FINE4", 1),
        "OSC 4 Fine Tune", -0.09f, 0.09f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("TONE1", 1),
        "OSC 1 Note", -11, 11, 0));
    
    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("TONE2", 1),
        "OSC 2 Note", -11, 11, 0));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("TONE3", 1),
        "OSC 3 Note", -11, 11, 0));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("TONE4", 1),
        "OSC 4 Note", -11, 11, 0));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OCT1", 1),
        "OSC 1 Octave", -4, 4, 0));
    
    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OCT2", 1),
        "OSC 2 Octave", -4, 4, 0));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OCT3", 1),
        "OSC 3 Octave", -4, 4, 0));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OCT4", 1),
        "OSC 4 Octave", -4, 4, 0));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VOL1", 1),
        "OSC 1 Volume", 0.0f, 1.0f, 0.6f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VOL2", 1),
        "OSC 2 Volume", 0.0f, 1.0f, 0.0f));
    
    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VOL3", 1),
        "OSC 3 Volume", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VOL4", 1),
        "OSC 4 Volume", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("SHAP", 1),
        "ENV shape", 0, 23, 15));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("MOAP", 1),
        "MOD ENV shape", 0, 23, 15));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OS1D", 1),
        "OSC 1 Detune", 0, 31, 3));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OS2D", 1),
        "OSC 2 Detune", 0, 31, 3));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OS3D", 1),
        "OSC 3 Detune", 0, 31, 3));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("OS4D", 1),
        "OSC 4 Detune", 0, 31, 3));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("OS1N", 1),
        "OSC 1 Voice", numv, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("OS2N", 1),
        "OSC 2 Voice", numv, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("OS3N", 1),
        "OSC 3 Voice", numv, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("OS4N", 1),
        "OSC 4 Voice", numv, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("WAVE1", 1),
        "OSC1 Waveform", Waveforms, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("WAVE2", 1),
        "OSC2 Waveform", Waveforms, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("WAVE3", 1),
        "OSC3 Waveform", Waveforms, 0));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("WAVE4", 1),
        "OSC4 Waveform", Waveforms, 0));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VIV1", 1),
        "OSC 1 Vibrato", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VIS1", 1),
        "Vibrato Speed", 0.0f, 100.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VIV2", 1),
        "OSC 2 Vibrato", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VIV3", 1),
        "OSC 3 Vibrato", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("VIV4", 1),
        "OSC 4 Vibrato", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FM1", 1),
        "OSC 1 FM from OSC 2", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FM2", 1),
        "OSC 2 FM from OSC 3", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FM3", 1),
        "OSC 3 FM from OSC 4", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("FM4", 1),
        "OSC 4 FM from OSC 1", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("DELA", 1),
        "Delay Volume", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("DELAF", 1),
        "Delay Feedback", 0.0f, 1.0f, 0.4f));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("DELaL", 1),
        "Delay Time L", 1, 16, 3));

    leyout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID ("DELaR", 1),
        "Delay Time R", 1, 16, 6));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("DELAFC", 1),
        "Delay Filter Cutoff", 0.0f, 1.0f, 0.2f));

    leyout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID ("DELAFT", 1),
        "Delay Filter Type", Ftype, 1));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("REVEFC", 1),
        "Reverb Feedback", 0.0f, 0.98f, 0.8f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("REVEV", 1),
        "Reverb Volume", 0.0f, 1.0f, 0.0f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("REVEH", 1),
        "Reverb High Cutoff", 0.0f, 1.0f, 0.8f));

    leyout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID ("REVEL", 1),
        "Reverb Low Cutoff", 0.0f, 0.05f, 0.01f));

    return leyout;
}

//🎹⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝🎹//
class AudioPluginAudioProcessor;
class synth : public juce::SynthesiserSound
{
public:
    bool appliesToChannel (int /*midiChannel*/) override { return true; } 
    bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
};

class Wave : public juce::SynthesiserVoice
{

public: Wave (AudioPluginAudioProcessor& p) : Moldfairy2 (p) 
{

    for (int i=0; i<8; ++i){
        OSCdata1[i]=0.0f;
        OSCdata2[i]=0.0f;
        OSCdata3[i]=0.0f;
        OSCdata4[i]=0.0f;
        OSC1U[i]=0.0f;
        OSC2U[i]=0.0f;
        OSC3U[i]=0.0f;
        OSC4U[i]=0.0f;
    }
    for (int i=0; i<4; ++i){
    voicegain[i]=0;
    }
    auto attack = Moldfairy2.atta->load();
    auto sustin = Moldfairy2.sust->load();
    auto decay = Moldfairy2.deca->load();
    auto release = Moldfairy2.rele->load();

    juce::ADSR::Parameters para1;
    para1.attack = attack;
    para1.decay = decay;
    para1.sustain = sustin;
    para1.release = release;
    ENV.setParameters (para1);

    auto Fattack = Moldfairy2.Ftta->load();
    auto Fsustin = Moldfairy2.Fust->load();
    auto Fdecay = Moldfairy2.Feca->load();
    auto Frelease = Moldfairy2.Fele->load();

    juce::ADSR::Parameters para2;
    para2.attack = Fattack;
    para2.decay = Fdecay;
    para2.sustain = Fsustin;
    para2.release = Frelease;
    FENV.setParameters (para2);
}

void cullentENV (float A, float D, float S, float R, float FA, float FD, float FS, float FR)
{
    juce::ADSR::Parameters para1;
    para1.attack = A;
    para1.decay = D;
    para1.sustain = S;
    para1.release = R;
    ENV.setParameters(para1);

    juce::ADSR::Parameters para2;
    para2.attack = FA;
    para2.decay = FD;
    para2.sustain = FS;
    para2.release = FR;
    FENV.setParameters(para2);
}

bool canPlaySound (juce::SynthesiserSound* sound) override
{ 
    return true; 
}

void startNote (int Notenum, float Velocity, juce::SynthesiserSound*, int) override
{
    
    float Rate = (float)getSampleRate();

        int Note1 = static_cast<int>(Moldfairy2.tone1->load());
        int Oct1 = 12 * static_cast<int>(Moldfairy2.oct1->load());
        float Frq1 = juce::MidiMessage::getMidiNoteInHertz(Notenum + Note1 + Oct1);
        float FIne1 = Moldfairy2.fine1->load();
        float frq1 = Frq1 * (1 + FIne1);
        PH1 = frq1 / Rate;
    
    
        int Note2 = static_cast<int>(Moldfairy2.tone2->load());
        int Oct2 = 12 * static_cast<int>(Moldfairy2.oct2->load());
        float Frq2 = juce::MidiMessage::getMidiNoteInHertz(Notenum + Note2 + Oct2);
        float FIne2 = Moldfairy2.fine2->load();
        float frq2 = Frq2 * (1 + FIne2);
        PH2 = frq2 / Rate;
    

        int Note3 = static_cast<int>(Moldfairy2.tone3->load());
        int Oct3 = 12 * static_cast<int>(Moldfairy2.oct3->load());
        float Frq3 = juce::MidiMessage::getMidiNoteInHertz(Notenum + Note3 + Oct3);
        float FIne3 = Moldfairy2.fine3->load();
        float frq3 = Frq3 * (1 + FIne3);
        PH3 = frq3 / Rate;
  
    
        int Note4 = static_cast<int>(Moldfairy2.tone4->load());
        int Oct4 = 12 * static_cast<int>(Moldfairy2.oct4->load());
        float Frq4 = juce::MidiMessage::getMidiNoteInHertz(Notenum + Note4 + Oct4);
        float FIne4 = Moldfairy2.fine4->load();
        float frq4 = Frq4 * (1 + FIne4);
        PH4 = frq4 / Rate;
    
    
        float vivfrq = Moldfairy2.OSCVIS->load();
        vivla = vivfrq / Rate;

    Moldfairy2.envshap = (int)24 * (static_cast<int>(Moldfairy2.shapetable->load()));
   
    Moldfairy2.Modenvshap = (int)24 * (static_cast<int>(Moldfairy2.Mshapetable->load()));

    int numV = static_cast<int>(Moldfairy2.numV->load());
    int numV2 = static_cast<int>(Moldfairy2.numV2->load());
    int numV3 = static_cast<int>(Moldfairy2.numV3->load());
    int numV4 = static_cast<int>(Moldfairy2.numV4->load());

    voicegain[0]=numV;voicegain[1]=numV2;
    voicegain[2]=numV3;voicegain[3]=numV4;

    numvoice = (int)(8 * numV);
    numvoice2 = (int)(8 * numV2);
    numvoice3 = (int)(8 * numV3);
    numvoice4 = (int)(8 * numV4);
    
    int Dtune = static_cast<int>(Moldfairy2.dtun->load());
    int Dtune2 = static_cast<int>(Moldfairy2.dtun2->load());
    int Dtune3 = static_cast<int>(Moldfairy2.dtun3->load());
    int Dtune4 = static_cast<int>(Moldfairy2.dtun4->load());

    OSC1U[0]=PH1;OSC1U[1]=PH1;
    OSC2U[0]=PH2;OSC2U[1]=PH2;
    OSC3U[0]=PH3;OSC3U[1]=PH3;
    OSC4U[0]=PH4;OSC4U[1]=PH4;

    for (int i=0; i<6; ++i){
        Damo = Moldfairy2.DetunAmo[Dtune];
        Damo2 = Moldfairy2.DetunAmo[Dtune2];
        Damo3 = Moldfairy2.DetunAmo[Dtune3];
        Damo4 = Moldfairy2.DetunAmo[Dtune4];
        OSC1U[2+i]=OSC1U[1+i]*Damo;
        OSC2U[2+i]=OSC2U[1+i]*Damo2;
        OSC3U[2+i]=OSC3U[1+i]*Damo3;
        OSC4U[2+i]=OSC4U[1+i]*Damo4;
    }
   
    ENV.noteOn();
    FENV.noteOn();
}

void stopNote (float Velocity, bool allowTailOff) override
{
    ENV.noteOff();
    FENV.noteOff();
    if(!allowTailOff)clearCurrentNote();
}

void pitchWheelMoved (int) override 
{
    
}
void controllerMoved (int, int) override 
{

}

void renderNextBlock (AudioBuffer<float> &output, int startSample, int numSamples) override
{
    float volme1 = Moldfairy2.vol1->load();
    float volme2 = Moldfairy2.vol2->load();
    float volme3 = Moldfairy2.vol3->load();
    float volme4 = Moldfairy2.vol4->load();

    osc1wave = (int)(32 * static_cast<int>(Moldfairy2.OSC1WAVE->load()));
    osc2wave = (int)(32 * static_cast<int>(Moldfairy2.OSC2WAVE->load()));
    osc3wave = (int)(32 * static_cast<int>(Moldfairy2.OSC3WAVE->load()));
    osc4wave = (int)(32 * static_cast<int>(Moldfairy2.OSC4WAVE->load()));

    float vivamo1 = Moldfairy2.OSC1VIV->load() * 0.1f;
    float vivamo2 = Moldfairy2.OSC2VIV->load() * 0.1f;
    float vivamo3 = Moldfairy2.OSC3VIV->load() * 0.1f;
    float vivamo4 = Moldfairy2.OSC4VIV->load() * 0.1f;

    float FMamo1 = Moldfairy2.FMM1->load();
    float FMamo2 = Moldfairy2.FMM2->load();
    float FMamo3 = Moldfairy2.FMM3->load();
    float FMamo4 = Moldfairy2.FMM4->load();

    
       
    auto* chL = output.getWritePointer (0, startSample);
    auto* chR = output.getNumChannels() > 1 ? output.getWritePointer (1, startSample) : chL;

    for (int i=0; i<numSamples; ++i){
        float ENVs = ENV.getNextSample(); 
        float VolENV = Moldfairy2.ENVtypes[((int)(ENVs*23))+Moldfairy2.envshap];
        float Menv = FENV.getNextSample();
        modENV = Moldfairy2.ENVtypes[((int)(Menv*23))+Moldfairy2.Modenvshap];
        
        vivData += vivla;
        if (vivData >= 1.0f)vivData -= 1.0f;
        float viv = Moldfairy2.WaveTable[((int)(vivData*31))+32];

        float out1[2]; float out2[2];
        float out3[2]; float out4[2];

        for (int y=0; y<2; ++y){
            out1[y]=0.0f;
            out2[y]=0.0f;
            out3[y]=0.0f;
            out4[y]=0.0f;
        }

        for (int x=0;x<8;++x){

            float viv4= 1.0f-(viv*vivamo4);
            float fm4= 1.0f-(OSCdata1[x]*FMamo4);
            OSCdata4[x]+=(OSC4U[x]*viv4)*fm4;
            if(OSCdata4[x]>=1.0f)OSCdata4[x]-=1.0f;
            out4[(x+2)%2]+=Moldfairy2.WaveTable[((int)(OSCdata4[x]*31))+osc4wave]*oscindex[x+numvoice4];
         

            float viv3= 1.0f-(viv*vivamo3);
            float fm3= 1.0f-(OSCdata4[x]*FMamo3);
            OSCdata3[x]+=(OSC3U[x]*viv3)*fm3;
            if(OSCdata3[x]>=1.0f)OSCdata3[x]-=1.0f;
            out3[(x+2)%2]+=Moldfairy2.WaveTable[((int)(OSCdata3[x]*31))+osc3wave]*oscindex[x+numvoice3];

            float viv2= 1.0f-(viv*vivamo2);
            float fm2= 1.0f-(OSCdata3[x]*FMamo2);
            OSCdata2[x]+=(OSC2U[x]*viv2)*fm2;
            if(OSCdata2[x]>=1.0f)OSCdata2[x]-=1.0f;
            out2[(x+2)%2]+=Moldfairy2.WaveTable[((int)(OSCdata2[x]*31))+osc2wave]*oscindex[x+numvoice2];

            float viv1= 1.0f-(viv*vivamo1);
            float fm1= 1.0f-(OSCdata2[x]*FMamo1);
            OSCdata1[x]+=(OSC1U[x]*viv1)*fm1;
            if(OSCdata1[x]>=1.0f)OSCdata1[x]-=1.0f;
            out1[(x+2)%2]+=Moldfairy2.WaveTable[((int)(OSCdata1[x]*31))+osc1wave]*oscindex[x+numvoice];

        }

        float waveout[2];waveout[0]=0.0f;waveout[1]=0.0f;

        for (int z=0;z<2;++z){
            waveout[z]=((out1[z]*gainfix[voicegain[0]]*volme1)+(out2[z]*gainfix[voicegain[1]]*volme2)+(out3[z]*gainfix[voicegain[2]]*volme3)+(out4[z]*gainfix[voicegain[3]]*volme4))*VolENV;
        }

        chL[i] += waveout[0];
        chR[i] += waveout[1];

    }

}   

    int osc1wave =0;
    int osc2wave =0;
    int osc3wave =0;
    int osc4wave =0;

    float vivla = 0.0f;
    float vivData = 0.0f;

    float modENV = 0.0f;

    int numvoice =0;
    int numvoice2 =0;
    int numvoice3 =0;
    int numvoice4 =0;

    int voicegain[4];

    float Damo = 0.0f;
    float Damo2 = 0.0f;
    float Damo3 = 0.0f;
    float Damo4 = 0.0f;

    float PH1 = 0.0f;
    float PH2 = 0.0f;
    float PH3 = 0.0f;
    float PH4 = 0.0f;

    float PHW = 0.0f;

    float OSCdata1[8];
    float OSCdata2[8];
    float OSCdata3[8];
    float OSCdata4[8];

    float OSC1U[8];
    float OSC2U[8];
    float OSC3U[8];
    float OSC4U[8];

    static inline const int oscindex[]{
    1,1,0,0,0,0,0,0,
    0,0,1,1,0,0,0,0,
    1,1,1,1,0,0,0,0,
    0,0,1,1,1,1,0,0,
    1,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1
    };

    static inline const float gainfix[]{
        1.0f,
        0.9f,
        0.75f,
        0.7f,
        0.65f,
        0.6f,
        0.6f
    };

private:

    juce::ADSR ENV;

    juce::ADSR FENV;

AudioPluginAudioProcessor& Moldfairy2; 
};
//🎹⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝🎹//


void AudioPluginAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    float sample = getSampleRate();
    if (parameterID == "ATTA" || parameterID == "SUST" || parameterID == "DECA" || parameterID == "RELE" || parameterID == "FIAT" || parameterID == "FISU" || parameterID == "FIDE" || parameterID == "FILE")
    {
        auto attack = atta->load();
        auto sustin = sust->load();
        auto decay = deca->load();
        auto release = rele->load();

        auto Fattack = Ftta->load();
        auto Fsustin = Feca->load();
        auto Fdecay = Fust->load();
        auto Frelease = Fele->load();
    
        for (int i = 0; i < Synth.getNumVoices(); ++i){
            if (auto* Voice = dynamic_cast<Wave*>(Synth.getVoice(i))){ 
            Voice->cullentENV(attack, decay, sustin, release, Fattack, Fdecay, Fsustin, Frelease);
    }}}

    if (parameterID == "SHAP"){
        envshap = ((int)24 * (int)newValue);
    }

    if (parameterID == "MOAP"){
        Modenvshap = ((int)24 * (int)newValue);
    }
}

void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //juce::ignoreUnused (sampleRate, samplesPerBlock);
    shapetable = STATES.getRawParameterValue("SHAP");
    Mshapetable = STATES.getRawParameterValue("MOAP");

    culcut = STATES.getRawParameterValue("CUTO");
    culres = STATES.getRawParameterValue("RESO");
    culcmo = STATES.getRawParameterValue("CUMO");
    culrmo = STATES.getRawParameterValue("REMO");

    tone1 = STATES.getRawParameterValue("TONE1");
    tone2 = STATES.getRawParameterValue("TONE2");
    tone3 = STATES.getRawParameterValue("TONE3");
    tone4= STATES.getRawParameterValue("TONE4");

    oct1 = STATES.getRawParameterValue("OCT1");
    oct2 = STATES.getRawParameterValue("OCT2");
    oct3 = STATES.getRawParameterValue("OCT3");
    oct4 = STATES.getRawParameterValue("OCT4");

    vol1 = STATES.getRawParameterValue("VOL1");
    vol2 = STATES.getRawParameterValue("VOL2");
    vol3 = STATES.getRawParameterValue("VOL3");
    vol4 = STATES.getRawParameterValue("VOL4");

    fine1 = STATES.getRawParameterValue("FINE1");
    fine2 = STATES.getRawParameterValue("FINE2");
    fine3 = STATES.getRawParameterValue("FINE3");
    fine4 = STATES.getRawParameterValue("FINE4");

    atta = STATES.getRawParameterValue("ATTA");
    sust = STATES.getRawParameterValue("SUST");
    deca = STATES.getRawParameterValue("DECA");
    rele = STATES.getRawParameterValue("RELE");

    Ftta = STATES.getRawParameterValue("FIAT");
    Fust = STATES.getRawParameterValue("FISU");
    Feca = STATES.getRawParameterValue("FIDE");
    Fele = STATES.getRawParameterValue("FIRE");

    dtun = STATES.getRawParameterValue("OS1D");
    numV = STATES.getRawParameterValue("OS1N");

    dtun3 = STATES.getRawParameterValue("OS3D");
    numV3 = STATES.getRawParameterValue("OS3N");

    dtun2 = STATES.getRawParameterValue("OS2D");
    numV2 = STATES.getRawParameterValue("OS2N");

    dtun4 = STATES.getRawParameterValue("OS4D");
    numV4 = STATES.getRawParameterValue("OS4N");

    OSC1WAVE = STATES.getRawParameterValue("WAVE1");
    OSC2WAVE = STATES.getRawParameterValue("WAVE2");
    OSC3WAVE = STATES.getRawParameterValue("WAVE3");
    OSC4WAVE = STATES.getRawParameterValue("WAVE4");

    OSC1VIV = STATES.getRawParameterValue("VIV1");
    OSC2VIV = STATES.getRawParameterValue("VIV2");
    OSC3VIV = STATES.getRawParameterValue("VIV3");
    OSC4VIV = STATES.getRawParameterValue("VIV4");

    OSCVIS = STATES.getRawParameterValue("VIS1");

    FMM1 = STATES.getRawParameterValue("FM1");
    FMM3 = STATES.getRawParameterValue("FM3");
    FMM2 = STATES.getRawParameterValue("FM2");
    FMM4 = STATES.getRawParameterValue("FM4");

    DELAYF = STATES.getRawParameterValue("DELAF");
    
    DELAYV = STATES.getRawParameterValue("DELA");
    DELAYFIL = STATES.getRawParameterValue("DELAFC");
    DELAYFILT = STATES.getRawParameterValue("DELAFT");

    DELAyL = STATES.getRawParameterValue("DELaL");
    DELAyR = STATES.getRawParameterValue("DELaR");

    REVEfc = STATES.getRawParameterValue("REVEFC");
    REVEv = STATES.getRawParameterValue("REVEV");

    REVEh = STATES.getRawParameterValue("REVEH");
    REVEl = STATES.getRawParameterValue("REVEL");

    std::fill(std::begin(filter),std::end(filter),0.0f);
    std::fill(std::begin(filtera),std::end(filtera),0.0f);
    std::fill(std::begin(filterb),std::end(filterb),0.0f);
    std::fill(std::begin(filterc),std::end(filterc),0.0f);
    std::fill(std::begin(filterd),std::end(filterd),0.0f);

    std::fill(std::begin(delay),std::end(delay),0.0f);
    std::fill(std::begin(delayR),std::end(delayR),0.0f);

    std::fill(std::begin(combL),std::end(combL),0.0f);
    std::fill(std::begin(combR),std::end(combR),0.0f);
    std::fill(std::begin(combL2),std::end(combL2),0.0f);
    std::fill(std::begin(combR2),std::end(combR2),0.0f);
    std::fill(std::begin(combL3),std::end(combL3),0.0f);
    std::fill(std::begin(combR3),std::end(combR3),0.0f);
    std::fill(std::begin(combL4),std::end(combL4),0.0f);
    std::fill(std::begin(combR4),std::end(combR4),0.0f);
    std::fill(std::begin(combL5),std::end(combL5),0.0f);
    std::fill(std::begin(combR5),std::end(combR5),0.0f);
    std::fill(std::begin(combL6),std::end(combL6),0.0f);
    std::fill(std::begin(combR6),std::end(combR6),0.0f);
    std::fill(std::begin(combL7),std::end(combL7),0.0f);
    std::fill(std::begin(combR7),std::end(combR7),0.0f);
    std::fill(std::begin(combL8),std::end(combL8),0.0f);
    std::fill(std::begin(combR8),std::end(combR8),0.0f);

    std::fill(std::begin(allpasL),std::end(allpasL),0.0f);
    std::fill(std::begin(allpasR),std::end(allpasR),0.0f);
    std::fill(std::begin(allpasL2),std::end(allpasL2),0.0f);
    std::fill(std::begin(allpasR2),std::end(allpasR2),0.0f);
    std::fill(std::begin(allpasL3),std::end(allpasL3),0.0f);
    std::fill(std::begin(allpasR3),std::end(allpasR3),0.0f);
    std::fill(std::begin(allpasL4),std::end(allpasL4),0.0f);
    std::fill(std::begin(allpasR4),std::end(allpasR4),0.0f);
    std::fill(std::begin(allpasL5),std::end(allpasL5),0.0f);
    std::fill(std::begin(allpasR5),std::end(allpasR5),0.0f);
    std::fill(std::begin(allpasL6),std::end(allpasL6),0.0f);
    std::fill(std::begin(allpasR6),std::end(allpasR6),0.0f);

    delayindex =0;
    combindx =0;
    apfindx =0;

    Synth.setCurrentPlaybackSampleRate(sampleRate);
    Synth.clearSounds();
    Synth.clearVoices();
    for (int i = 0; i < 4; ++i){
    
        Synth.addVoice (new Wave(*this));
    }
        Synth.addSound (new synth());   

}


void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    float cutoff = culcut->load();
    float resona = culres->load();
    float cutmod = culcmo->load();
    float resomo = culrmo->load();
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto sample = buffer.getNumSamples();

    float delayFback = DELAYF->load();
    
    float delayVolum = DELAYV->load();

    float reverbFback = 0.4f;
    float reverbFbackComb = REVEfc->load();
    float reverbVolum = REVEv->load();
    float reverbVolumComb = 0.7f;

    float reverbHcut = 1.0f-REVEh->load();
    float reverbLcut = 1.0f-REVEl->load();

    int delayLtime = static_cast<int>(DELAyL->load());
    int delayRtime = static_cast<int>(DELAyR->load());

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, sample);
    }

    if (auto* playHead = getPlayHead()){
        if (auto posi = playHead->getPosition()){
            if (posi->getBpm().hasValue()){
                BPM = static_cast<float>(*(posi->getBpm()));
            }
        }
    }
        float delayS = 60 / BPM;
        int delaynumsample = static_cast<int>((delayS*0.25) * getSampleRate());

        int delayRsamp = jmin((delaynumsample*delayRtime), 88199);
        int delayLsamp = jmin((delaynumsample*delayLtime), 88199);
     
        float delayfilcut = 1.0f-DELAYFIL->load();
        int dftype = static_cast<int>(DELAYFILT->load());
    
    Synth.renderNextBlock(buffer, midiMessages, 0, sample);
    auto* chL = buffer.getWritePointer (0);
    auto* chR = buffer.getNumChannels() > 1 ? buffer.getWritePointer (1) : chL;
    
    for (int i=0; i<sample; ++i){
        float MENV = 0.0f;
        for (int v=0; v<Synth.getNumVoices(); ++v){
            if (auto Menv = dynamic_cast<Wave*>(Synth.getVoice(v))){
                if (MENV < (Menv->modENV)){
                    MENV = (Menv->modENV);
                }
            }
        }
        
        float reso = resona + (resomo * MENV);
        float cutoo = 1.0f-(cutoff+(MENV*cutmod));

        if (reso > 1.0f)reso = 1.0f;
        if (reso < 0.0f)reso = 0.0f;
        if (cutoo > 1.0f)cutoo = 1.0f;
        if (cutoo < 0.0f)cutoo = 0.0f;

        float fL = (filtera[22]*-1.4f);
        float fR = (filtera[23]*-1.4f);
        float feedL = fL / (1.0f + std::fabs(fL));
        float feedR = fR / (1.0f + std::fabs(fR));

        float culinp = chL[i]+(feedL*reso);
        float culinpR = chR[i]+(feedR*reso);

        float out1 = (1.0f-cutoo)*culinp+(cutoo*filter[0]);
        float out2 = (1.0f-cutoo)*culinpR+(cutoo*filter[1]);

        filter[0]=out1;
        filter[1]=out2;

        float out3 = (1.0f-cutoo)*filter[0]+(cutoo*filter[2]);
        float out4 = (1.0f-cutoo)*filter[1]+(cutoo*filter[3]);

        filter[2]=out3;
        filter[3]=out4;

        float out5 = (1.0f-cutoo)*filter[2]+(cutoo*filter[4]);
        float out6 = (1.0f-cutoo)*filter[3]+(cutoo*filter[5]);

        filter[4]=out5;
        filter[5]=out6;

        float out7 = (1.0f-cutoo)*filter[4]+(cutoo*filter[6]);
        float out8 = (1.0f-cutoo)*filter[5]+(cutoo*filter[7]);

        filter[6]=out7;
        filter[7]=out8;

        float out9 = (1.0f-cutoo)*filter[6]+(cutoo*filter[8]);
        float out10 = (1.0f-cutoo)*filter[7]+(cutoo*filter[9]);

        filter[8]=out9;
        filter[9]=out10;

        float out11 = (1.0f-cutoo)*filter[8]+(cutoo*filter[10]);
        float out12 = (1.0f-cutoo)*filter[9]+(cutoo*filter[11]);

        filter[10]=out11;
        filter[11]=out12;

        float out13 = (1.0f-cutoo)*filter[10]+(cutoo*filter[12]);
        float out14 = (1.0f-cutoo)*filter[11]+(cutoo*filter[13]);

        filter[12]=out13;
        filter[13]=out14;

        float out15 = (1.0f-cutoo)*filter[12]+(cutoo*filter[14]);
        float out16 = (1.0f-cutoo)*filter[13]+(cutoo*filter[15]);

        filter[14]=out15;
        filter[15]=out16;

        float out17 = (1.0f-cutoo)*filter[14]+(cutoo*filter[16]);
        float out18 = (1.0f-cutoo)*filter[15]+(cutoo*filter[17]);

        filter[16]=out17;
        filter[17]=out18;

        float out19 = (1.0f-cutoo)*filter[16]+(cutoo*filter[18]);
        float out20 = (1.0f-cutoo)*filter[17]+(cutoo*filter[19]);

        filter[18]=out19;
        filter[19]=out20;

        float out21 = (1.0f-cutoo)*filter[18]+(cutoo*filter[20]);
        float out22 = (1.0f-cutoo)*filter[19]+(cutoo*filter[21]);

        filter[20]=out21;
        filter[21]=out22;

        float out23 = (1.0f-cutoo)*filter[20]+(cutoo*filter[22]);
        float out24 = (1.0f-cutoo)*filter[21]+(cutoo*filter[23]);

        filter[22]=out23;
        filter[23]=out24;

        filtera[0]=out23;
        filtera[1]=out24;

        float out3a = (1.0f-cutoo)*filtera[0]+(cutoo*filtera[2]);
        float out4a = (1.0f-cutoo)*filtera[1]+(cutoo*filtera[3]);

        filtera[2]=out3a;
        filtera[3]=out4a;

        float out5a = (1.0f-cutoo)*filtera[2]+(cutoo*filtera[4]);
        float out6a = (1.0f-cutoo)*filtera[3]+(cutoo*filtera[5]);

        filtera[4]=out5a;
        filtera[5]=out6a;

        float out7a = (1.0f-cutoo)*filtera[4]+(cutoo*filtera[6]);
        float out8a = (1.0f-cutoo)*filtera[5]+(cutoo*filtera[7]);

        filtera[6]=out7a;
        filtera[7]=out8a;

        float out9a = (1.0f-cutoo)*filtera[6]+(cutoo*filtera[8]);
        float out10a = (1.0f-cutoo)*filtera[7]+(cutoo*filtera[9]);

        filtera[8]=out9a;
        filtera[9]=out10a;

        float out11a = (1.0f-cutoo)*filtera[8]+(cutoo*filtera[10]);
        float out12a = (1.0f-cutoo)*filtera[9]+(cutoo*filtera[11]);

        filtera[10]=out11a;
        filtera[11]=out12a;

        float out13a = (1.0f-cutoo)*filtera[10]+(cutoo*filtera[12]);
        float out14a = (1.0f-cutoo)*filtera[11]+(cutoo*filtera[13]);

        filtera[12]=out13a;
        filtera[13]=out14a;

        float out15a = (1.0f-cutoo)*filtera[12]+(cutoo*filtera[14]);
        float out16a = (1.0f-cutoo)*filtera[13]+(cutoo*filtera[15]);

        filtera[14]=out15a;
        filtera[15]=out16a;

        float out17a = (1.0f-cutoo)*filtera[14]+(cutoo*filtera[16]);
        float out18a = (1.0f-cutoo)*filtera[15]+(cutoo*filtera[17]);

        filtera[16]=out17a;
        filtera[17]=out18a;

        float out19a = (1.0f-cutoo)*filtera[16]+(cutoo*filtera[18]);
        float out20a = (1.0f-cutoo)*filtera[17]+(cutoo*filtera[19]);

        filtera[18]=out19a;
        filtera[19]=out20a;

        float out21a = (1.0f-cutoo)*filtera[18]+(cutoo*filtera[20]);
        float out22a = (1.0f-cutoo)*filtera[19]+(cutoo*filtera[21]);

        filtera[20]=out21a;
        filtera[21]=out22a;

        float out23a = (1.0f-cutoo)*filtera[20]+(cutoo*filtera[22]);
        float out24a = (1.0f-cutoo)*filtera[21]+(cutoo*filtera[23]);

        filtera[22]=out23a;
        filtera[23]=out24a;

        chL[i]=out23a;
        chR[i]=out24a;

        int readIndex = (delayindex - delayLsamp+88200)%88200;
            
        int readIndexR = (delayindex - delayRsamp+88200)%88200;

        float Lwet = delay[readIndex];
        float Rwet = delayR[readIndexR];
        
        float Dout1 = (1.0f-delayfilcut)*Lwet+(delayfilcut*filterb[0]);
        float Dout2 = (1.0f-delayfilcut)*Rwet+(delayfilcut*filterb[1]);
        
        filterb[0]=Dout1;
        filterb[1]=Dout2;

        float Dout3 = delayfilcut*(filterb[2]+Lwet-filterb[3]);
        float Dout4 = delayfilcut*(filterb[4]+Rwet-filterb[5]);

        filterb[2]=Dout3;
        filterb[3]=Lwet;
        filterb[4]=Dout4;
        filterb[5]=Rwet;
        
        delay[delayindex] = chL[i] + (((Dout3*dftype)+(Dout1*(1-dftype))*0.6f) * delayFback);
        delayR[delayindex] = chR[i] + (((Dout4*dftype)+(Dout2*(1-dftype))*0.6f) * delayFback);

        chL[i] = chL[i] + (((Dout3*dftype)+(Dout1*(1-dftype)))*delayVolum);
        chR[i] = chR[i] + (((Dout4*dftype)+(Dout2*(1-dftype)))*delayVolum);

        LFO1 += 0.0000052154f;
        if (LFO1>=1.0f)LFO1-=1.0f; 
        LFO2 += 0.00000839f;
        if (LFO2>=1.0f)LFO2-=1.0f;
        LFO3 += 0.0000115646f;
        if (LFO3>=1.0f)LFO3-=1.0f;
        LFO4 += 0.0000156463f;
        if (LFO4>=1.0f)LFO4-=1.0f;

        int lfo1 = ReverbLFO[((int)LFO1*35)];
        int lfo2 = ReverbLFO[((int)LFO2*35)];
        int lfo3 = ReverbLFO[((int)LFO3*35)];
        int lfo4 = ReverbLFO[((int)LFO4*35)];

        int comb1 = (combindx - (1031+lfo1)+4096)%4096;

        int comb2 = (combindx - (1123+lfo2)+4096)%4096;
            
        int comb3 = (combindx - (1217+lfo3)+4096)%4096;
            
        int comb4 = (combindx - (1321+lfo4)+4096)%4096;
        
        int comb5 = (combindx - (1429+lfo1)+4096)%4096;
          
        int comb6 = (combindx - (1531+lfo2)+4096)%4096;
            
        int comb7 = (combindx - (1621+lfo3)+4096)%4096;
            
        int comb8 = (combindx - (1721+lfo4)+4096)%4096;
           
        int comb1a = (combindx - (1789+lfo1)+4096)%4096;
           
        int comb2a = (combindx - (1901+lfo2)+4096)%4096;
            
        int comb3a = (combindx - (1999+lfo3)+4096)%4096;
            
        int comb4a = (combindx - (2113+lfo4)+4096)%4096;

        int comb5a = (combindx - (2221+lfo1)+4096)%4096;
           
        int comb6a = (combindx - (2333+lfo2)+4096)%4096;
          
        int comb7a = (combindx - (2467+lfo3)+4096)%4096;
            
        int comb8a = (combindx - (2593+lfo4)+4096)%4096;
           
        
        int alp1 = (apfindx - 233 + 1024)%1024;
            
        int alp2 = (apfindx - 293 + 1024)%1024;
            
        int alp3 = (apfindx - 383 + 1024)%1024;
           
        int alp4 = (apfindx - 419 + 1024)%1024;
            
        int alp5 = (apfindx - 547 + 1024)%1024;
           
        int alp6 = (apfindx - 701 + 1024)%1024;
            

        float ainp1 = chL[i];
        float ainp2 = chR[i];
        float siga1 = allpasL[alp1];
        float siga2 = allpasR[alp1];
        float aout1 = (ainp1 *-reverbFback)+siga1;
        float aout2 = (ainp2 *-reverbFback)+siga2;
        allpasL[apfindx]=ainp1+(siga1*reverbFback);
        allpasR[apfindx]=ainp2+(siga2*reverbFback);

        float siga3 = allpasL2[alp2];
        float siga4 = allpasR2[alp2];
        float aout3 = (aout1 *-reverbFback)+siga3;
        float aout4 = (aout2 *-reverbFback)+siga4;
        allpasL2[apfindx]=aout1+(siga3*reverbFback);
        allpasR2[apfindx]=aout2+(siga4*reverbFback);

        float siga5 = allpasL3[alp3];
        float siga6 = allpasR3[alp3];
        float aout5 = (aout3 *-reverbFback)+siga5;
        float aout6 = (aout4 *-reverbFback)+siga6;
        allpasL3[apfindx]=aout3+(siga5*reverbFback);
        allpasR3[apfindx]=aout4+(siga6*reverbFback);

        float siga7 = allpasL4[alp4];
        float siga8 = allpasR4[alp4];
        float aout7 = (aout5 *-reverbFback)+siga7;
        float aout8 = (aout6 *-reverbFback)+siga8;
        allpasL4[apfindx]=aout5+(siga7*reverbFback);
        allpasR4[apfindx]=aout6+(siga8*reverbFback);

        float siga9 = allpasL5[alp5];
        float siga10 = allpasR5[alp5];
        float aout9 = (aout7 *-reverbFback)+siga9;
        float aout10 = (aout8 *-reverbFback)+siga10;
        allpasL5[apfindx]=aout7+(siga9*reverbFback);
        allpasR5[apfindx]=aout8+(siga10*reverbFback);

        float siga11 = allpasL6[alp6];
        float siga12 = allpasR6[alp6];
        float aout11 = (aout9 *-reverbFback)+siga11;
        float aout12 = (aout10 *-reverbFback)+siga12;
        allpasL6[apfindx]=aout9+(siga11*reverbFback);
        allpasR6[apfindx]=aout10+(siga12*reverbFback);
        
        float fd1 = combL[comb1];
        float fd2 = combL2[comb3];
        float fd3 = combL3[comb5];
        float fd4 = combL4[comb7];
        float fd5 = combL5[comb1a];
        float fd6 = combL6[comb3a];
        float fd7 = combL7[comb5a];
        float fd8 = combL8[comb7a];
        float fd9 = combR[comb2];
        float fd10 = combR2[comb4];
        float fd11 = combR3[comb6];
        float fd12 = combR4[comb8];
        float fd13 = combR5[comb2a];
        float fd14 = combR6[comb4a];
        float fd15 = combR7[comb6a];
        float fd16 = combR8[comb8a];

        float cout1 = (1.0f-reverbHcut)*fd1+(reverbHcut*filterc[0]);
        float cout2 = (1.0f-reverbHcut)*fd9+(reverbHcut*filterc[1]);

        filterc[0]=cout1;
        filterc[1]=cout2;

        float cout3 = (1.0f-reverbHcut)*fd2+(reverbHcut*filterc[2]);
        float cout4 = (1.0f-reverbHcut)*fd10+(reverbHcut*filterc[3]);

        filterc[2]=cout3;
        filterc[3]=cout4;

        float cout5 = (1.0f-reverbHcut)*fd3+(reverbHcut*filterc[4]);
        float cout6 = (1.0f-reverbHcut)*fd11+(reverbHcut*filterc[5]);

        filterc[4]=cout5;
        filterc[5]=cout6;

        float cout7 = (1.0f-reverbHcut)*fd4+(reverbHcut*filterc[6]);
        float cout8 = (1.0f-reverbHcut)*fd12+(reverbHcut*filterc[7]);

        filterc[6]=cout7;
        filterc[7]=cout8;

        float cout9 = (1.0f-reverbHcut)*fd5+(reverbHcut*filterc[8]);
        float cout10 = (1.0f-reverbHcut)*fd13+(reverbHcut*filterc[9]);

        filterc[8]=cout9;
        filterc[9]=cout10;

        float cout11 = (1.0f-reverbHcut)*fd6+(reverbHcut*filterc[10]);
        float cout12 = (1.0f-reverbHcut)*fd14+(reverbHcut*filterc[11]);

        filterc[10]=cout11;
        filterc[11]=cout12;

        float cout13 = (1.0f-reverbHcut)*fd7+(reverbHcut*filterc[12]);
        float cout14 = (1.0f-reverbHcut)*fd15+(reverbHcut*filterc[13]);

        filterc[12]=cout13;
        filterc[13]=cout14;

        float cout15 = (1.0f-reverbHcut)*fd8+(reverbHcut*filterc[14]);
        float cout16 = (1.0f-reverbHcut)*fd16+(reverbHcut*filterc[15]);

        filterc[14]=cout15;
        filterc[15]=cout16;

        
        float fcout1 = reverbLcut*(filterd[0]+cout1-filterd[1]);
        float fcout2 = reverbLcut*(filterd[2]+cout2-filterd[3]);

        filterd[0]=fcout1;
        filterd[1]=cout1;
        filterd[2]=fcout2;
        filterd[3]=cout2;

        float fcout3 = reverbLcut*(filterd[4]+cout3-filterd[5]);
        float fcout4 = reverbLcut*(filterd[6]+cout4-filterd[7]);

        filterd[4]=fcout3;
        filterd[5]=cout3;
        filterd[6]=fcout4;
        filterd[7]=cout4;

        float fcout5 = reverbLcut*(filterd[8]+cout5-filterd[9]);
        float fcout6 = reverbLcut*(filterd[10]+cout6-filterd[11]);

        filterd[8]=fcout5;
        filterd[9]=cout5;
        filterd[10]=fcout6;
        filterd[11]=cout6;

        float fcout7 = reverbLcut*(filterd[12]+cout7-filterd[13]);
        float fcout8 = reverbLcut*(filterd[14]+cout8-filterd[15]);

        filterd[12]=fcout7;
        filterd[13]=cout7;
        filterd[14]=fcout8;
        filterd[15]=cout8;

        float fcout9 = reverbLcut*(filterd[16]+cout9-filterd[17]);
        float fcout10 = reverbLcut*(filterd[18]+cout10-filterd[19]);

        filterd[16]=fcout9;
        filterd[17]=cout9;
        filterd[18]=fcout10;
        filterd[19]=cout10;

        float fcout11 = reverbLcut*(filterd[20]+cout11-filterd[21]);
        float fcout12 = reverbLcut*(filterd[22]+cout12-filterd[23]);

        filterd[20]=fcout11;
        filterd[21]=cout11;
        filterd[22]=fcout12;
        filterd[23]=cout12;

        float fcout13 = reverbLcut*(filterd[24]+cout13-filterd[25]);
        float fcout14 = reverbLcut*(filterd[26]+cout14-filterd[27]);

        filterd[24]=fcout13;
        filterd[25]=cout13;
        filterd[26]=fcout14;
        filterd[27]=cout14;

        float fcout15 = reverbLcut*(filterd[28]+cout15-filterd[29]);
        float fcout16 = reverbLcut*(filterd[30]+cout16-filterd[31]);

        filterd[28]=fcout15;
        filterd[29]=cout15;
        filterd[30]=fcout16;
        filterd[31]=cout16;

        float ham[16];
        ham[0]=fcout1; ham[1]=fcout2; ham[2]=fcout3; ham[3]=fcout4; ham[4]=fcout5; ham[5]=fcout6; ham[6]=fcout7; ham[7]=fcout8;
        ham[8]=fcout9; ham[9]=fcout10; ham[10]=fcout11; ham[11]=fcout12; ham[12]=fcout13; ham[13]=fcout14; ham[14]=fcout15; ham[15]=fcout16;
        Hadamard16(ham);
        
        combL[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[0]*reverbFbackComb;
        combR[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[1]*reverbFbackComb;
        combL2[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[2]*reverbFbackComb;
        combR2[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[3]*reverbFbackComb;
        combL3[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[4]*reverbFbackComb;
        combR3[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[5]*reverbFbackComb;
        combL4[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[6]*reverbFbackComb;
        combR4[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[7]*reverbFbackComb;
        combL5[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[8]*reverbFbackComb;
        combR5[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[9]*reverbFbackComb;
        combL6[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[10]*reverbFbackComb;
        combR6[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[11]*reverbFbackComb;
        combL7[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[12]*reverbFbackComb;
        combR7[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[13]*reverbFbackComb;
        combL8[combindx] = (aout11*(1.0f-(reverbFbackComb*0.4f))) + ham[14]*reverbFbackComb;
        combR8[combindx] = (aout12*(1.0f-(reverbFbackComb*0.4f))) + ham[15]*reverbFbackComb;

        float LoutFDN = (ham[0]+ham[2]+ham[4]+ham[6]+ham[8]+ham[11]+ham[13]+ham[15])*0.25f;
        float RoutFDN = (ham[1]+ham[3]+ham[5]+ham[7]+ham[9]+ham[10]+ham[12]+ham[14])*0.25f;

        chL[i] = (chL[i]*(1.0f-reverbVolum))+ ((LoutFDN*0.9f)*reverbVolum);
        chR[i] = (chR[i]*(1.0f-reverbVolum)) +((RoutFDN*0.9f)*reverbVolum);

        delayindex = (delayindex + 1) % 88200;
    
        combindx = (combindx + 1) % 4096;
        
        apfindx = (apfindx + 1) % 1024;
    }
    midiMessages.clear();
}

inline void AudioPluginAudioProcessor::Hadamard16 (float* Hamadard)
{
    for (int i = 0; i < 16; i += 2) {
        float a = Hamadard[i], b = Hamadard[i+1];
        Hamadard[i] = a + b; Hamadard[i+1] = a - b;
    }
    for (int i = 0; i < 16; i += 4) {
        for (int j = 0; j < 2; ++j) {
            float a = Hamadard[i+j], b = Hamadard[i+j+2];
            Hamadard[i+j] = a + b; Hamadard[i+j+2] = a - b;
        }
    }
    for (int i = 0; i < 16; i += 8) {
        for (int j = 0; j < 4; ++j) {
            float a = Hamadard[i+j], b = Hamadard[i+j+4];
            Hamadard[i+j] = a + b; Hamadard[i+j+4] = a - b;
        }
    }
    for (int j = 0; j < 8; ++j) {
        float a = Hamadard[j], b = Hamadard[j+8];
        Hamadard[j] = a + b; Hamadard[j+8] = a - b;
    }
    const float Nor = 1.0f / 4.0f; 
    for (int i = 0; i < 16; ++i) {
        Hamadard[i] *= Nor;
    }
}