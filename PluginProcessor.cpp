#pragma one

#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 1

#include "JuceHeader.h"

#include "PluginProcessor.h"

#include "PluginEditor.h"

#include "3bandseparator.h"

#include <string.h>

#include <algorithm>

#include <array>

#include <cmath>

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::paral()
{
    juce::AudioProcessorValueTreeState::ParameterLayout fs;

    for (int i=0; i<4; ++i){
        for (int x=0; x<9; ++x){
        juce:: String pids (oscpid[i][x]);
        float min = defval[x].min;
        float max = defval[x].max;
        float def = defval[x].def;
        bool ISint = defval[x].isint;
        if (ISint){
            fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID (pids, 1),
            pids, (int)min, (int)max, (int)def));
        }else{
            fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (pids, 1),
            pids, min, max, def));
        }
        }

    }
    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("note1", 1),
            "note1", -11, 11, 0));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("note2", 1),
            "note2", -11, 11, 0));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("note3", 1),
            "note3", -11, 11, 0));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("note4", 1),
            "note4", -11, 11, 0));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("test", 1),
            "noise", 0.0f, 1.0f, 0.1f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("biv", 1),
            "biv", 0.0f, 1.0f, 0.1f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("Fc", 1),
            "fitercut", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("Fr", 1),
            "fiterreso", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("FcM", 1),
            "fiterModCutoff", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("FrM", 1),
            "fiterModreso", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("att", 1),
            "attack", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("dec", 1),
            "decay", 0.0f, 1.0f, 0.2f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("sus", 1),
            "sustin", 0.0f, 1.0f, 0.2f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("rel", 1),
            "release", 0.0f, 1.0f, 0.1f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("matt", 1),
            "modattack", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("mdec", 1),
            "moddecay", 0.0f, 1.0f, 0.2f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("msus", 1),
            "modsustin", 0.0f, 1.0f, 0.2f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("mrel", 1),
            "modrelease", 0.0f, 1.0f, 0.1f));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("slo", 1),
            "LFO 1 (synced random) rate", 0, 7, 3));

    fs.add (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID ("slod", 1),
            "LFO 1 (synced random) dest", juce::StringArray{
                "none","cutoff","resonance"
            }, 0));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ulo", 1),
            "LFO 2 (unsynced, sine) rate", 0.0f, 1.0f, 0.1f));

    fs.add (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID ("ulod", 1),
            "LFO 2 (unsynced sine) dest", juce::StringArray{
                "none","cutoff","resonance"
            }, 0));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("sldp", 1),
            "LFO 1 depth", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("uldp", 1),
            "LFO 2 depth", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("wsm", 1),
            "waveshaper mix", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("wst", 1),
            "waveshaper type", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("wsg", 1),
            "waveshaper gain", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("cho", 1),
            "chorus mix", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("chd", 1),
            "chorus depth", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("chs", 1),
            "chorus speed", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("chw", 1),
            "chorus wide", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("DeL", 1),
            "delay L", 0, 15, 0));

    fs.add (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID ("DeR", 1),
            "delay R", 0, 15, 0));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("DeF", 1),
            "delay feedback", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("DeV", 1),
            "delay volume", 0.0f, 1.0f, 0.0f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ReF", 1),
            "reverb feedback", 0.0f, 1.0f, 0.9f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ReD", 1),
            "reverb deffusion", 0.0f, 1.0f, 0.7f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ReV", 1),
            "reverb volume", 0.0f, 1.0f, 0.0f));
    
    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ReLc", 1),
            "reverb lowcut", 0.0f, 1.0f, 0.4f));
    
    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("ReHc", 1),
            "reverb highcut", 0.0f, 1.0f, 0.3f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("compD", 1),
            "comp output gain", 0.0f, 1.0f, 0.5f));

    fs.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("comp", 1),
            "comp mix", 0.0f, 1.0f, 0.0f));

    return fs;

}
//==============================================================================
//⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜
OverTheTop::OverTheTop(){}

OverTheTop::~OverTheTop(){}

inline void OverTheTop::reset()
{ 
    compbuf=0.0f;

}

inline float OverTheTop::process(float in, float a, float r, float U, float D)
{
    a=0.999f+a*0.001f;r=0.999f+r*0.001f;

    float v = std::fabsf(in);

    v = F.process24(v, 0.7f, 2.0f, 1.0f, 0.0f, 0.0f);

    v = ((std::min(compbuf, v))*a)+v*(1.0f-a);
    compbuf = std::max(v, (compbuf*r));

    float compVol = (1.0f-compbuf);
    compVol=compVol*compVol*compVol;
    compbuf=1.0f-compVol;

    float Dg = std::max(0.0f,(compVol-0.3f))*-1.42f;
    float Ug = std::min(0.0f,(compVol-0.2f))*-5.0f;

    Dg*=D; Ug*=U;

    Dg+=1.0f;Ug+=1.0f;

    float out = in*Dg;
    out*=Ug;

    return out;
}

Filterdesing::Filterdesing(){
    low12=0.0f;band12=0.0f;high12=0.0f;
    low24=0.0f;band24=0.0f;high24=0.0f;
    L=0.0f; B=0.0f; H=0.0f;
}
Filterdesing::~Filterdesing(){}

inline float Filterdesing::process12(float in, float cut, float reso, float l, float b, float h)
{
    cut=cut*cut;
    cut*=0.96f;

    high12 = in - low12 - reso * band12;
    band12 = cut * high12 + band12;
    low12 = low12 + cut * band12;

    float out = ((low12*l)+(band12*b)+(high12*h));

    return out;

}

inline float Filterdesing::process24(float in, float cut, float reso, float l, float b, float h)
{
    cut=cut*cut;
    cut*=0.96f;

    H = in - L - reso * B;
    B = cut * H + B;
    L = L + cut * B;

    float out = ((L*l)+(B*b)+(H*h));

    float reso1 = std::max(1.0f, reso);

    high24 = out - low24 - reso1 * band24;
    band24 = cut * high24 + band24;
    low24 = low24 + cut * band24;

    out = ((low24*l)+(band24*b)+(high24*h));

    out=std::min(1.0f, (std::max(-1.0f, out)));

    return out;
}

class AudioPluginAudioProcessor;
class Synth : public juce::SynthesiserSound
{
public:
    bool appliesToChannel (int /*midiChannel*/) override { return true; } 
    bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
};

class Wave : public juce::SynthesiserVoice{
public: Wave (AudioPluginAudioProcessor& p) : processorRef (p) 
{
    Rate = (float)getSampleRate();
    oneHz = 1.0f / Rate;
    for (int i=0; i<4; ++i){
        for (int x=0; x<8; ++x){
            oscs[i][x]=0;
            oscdata[i][x]=0;
        }
    }
}

bool canPlaySound (juce::SynthesiserSound* sound) override {return true;}

void startNote (int Notenum, float Velocity, juce::SynthesiserSound*, int) override
{
    for (int i=0; i<4; ++i){
        int note = Notenum + (int)(processorRef.Par[i][1]->load()*12) + (int)(processorRef.pmt[38+i]->load());
        float fine = 1.0f + (processorRef.Par[i][2]->load()*0.1f);
        float pha = (oneHz * juce::MidiMessage::getMidiNoteInHertz(note))*fine;
        uint32_t phs = (uint32_t)(4294967296.0f * pha);
        oscs[i][0] =phs;oscs[i][1] =phs;
        float det = 1.0f - processorRef.Par[i][3]->load()*0.01f;
        for (int x=0; x<6; ++x){
            oscs[i][(x+2)]=(uint32_t)(oscs[i][(x+1)]*det);
        }
    }

    float vfrq = processorRef.pmt[0]->load()*63;
    int vivx = (int)vfrq;
    float vlip = vfrq - vivx;
    float vivrate = (processorRef.modcurv[vivx]*(1.0f-vlip))+processorRef.modcurv[(vivx+1)]*vlip;
    viv = (uint32_t)(vivrate * 4294967296.0f);

    float ar = (1.0f-processorRef.pmt[3]->load())*63;
    int atx = (int)ar;
    float lip = ar - atx;
    float attackrate = (processorRef.modcurv[atx]*(1.0f-lip))+processorRef.modcurv[(atx+1)]*lip;

    float de = ((1.0f-processorRef.pmt[4]->load()))*63;
    int dex = (int)de;
    float dlip = de - dex;
    float decayrate = (processorRef.modcurv[dex]*(1.0f-dlip))+processorRef.modcurv[(dex+1)]*dlip;

    float ar2 = (1.0f-processorRef.pmt[18]->load())*63;
    int atx2 = (int)ar2;
    float lip2 = ar2 - atx2;
    float attackrate2 = (processorRef.modcurv[atx2]*(1.0f-lip2))+processorRef.modcurv[(atx2+1)]*lip2;

    float de2 = ((1.0f-processorRef.pmt[19]->load()))*63;
    int dex2 = (int)de2;
    float dlip2 = de2 - dex2;
    float decayrate2 = (processorRef.modcurv[dex2]*(1.0f-dlip2))+processorRef.modcurv[(dex+1)]*dlip2;

    atta =1.0f-attackrate;
    deca =1.0f-decayrate;
    sust = processorRef.pmt[5]->load();
    rele = 1.0f;
    atta2 =1.0f-attackrate2;
    deca2 =1.0f-decayrate2;
    sust2 = processorRef.pmt[20]->load();
    rele2 = 1.0f;
    env = 1.0f;modenv = 1.0f;
    attack = 1.0f;Mattack = 1.0f;
    
    noteon = true;
}

void stopNote (float Velocity, bool allowTailOff) override
{
    noteon = false;
    deca = 1.0f;
    sust = 1.0f;
    atta = 1.0f;
    deca2 = 1.0f;
    sust2 = 1.0f;
    atta2 = 1.0f;
    float rr = ((1.0f-processorRef.pmt[6]->load()))*63;
    int rex = (int)rr;
    float rlip = rr - rex;
    float rlerate = (processorRef.modcurv[rex]*(1.0f-rlip))+processorRef.modcurv[(rex+1)]*rlip;
    rele = 1.0f-rlerate;
    float rr2 = ((1.0f-processorRef.pmt[21]->load()))*63;
    int rex2 = (int)rr2;
    float rlip2 = rr2 - rex2;
    float rlerate2 = (processorRef.modcurv[rex2]*(1.0f-rlip2))+processorRef.modcurv[(rex2+1)]*rlip2;
    rele = 1.0f-rlerate;
    rele2 = 1.0f-rlerate2;
}

void pitchWheelMoved (int) override {}
void controllerMoved (int, int) override {}

void renderNextBlock (AudioBuffer<float> &output, int startSample, int numSamples) override
{
    auto* chL = output.getWritePointer (0, startSample);
    auto* chR = output.getNumChannels() > 1 ? output.getWritePointer (1, startSample) : chL;

    int waveforme[4];
    int voicenum[4][8];
    float volume[4];
    float vivra[4];
    float fm[4];
    float stereo[4];
    for (int i=0; i<4; ++i){
        waveforme[i]=(int)processorRef.Par[i][5]->load();
        vivra[i]=processorRef.Par[i][8]->load()*0.1f;
        fm[i]=processorRef.Par[i][6]->load();
        volume[i]=processorRef.Par[i][0]->load();
        stereo[i]=processorRef.Par[i][7]->load();
        int numv =(int)processorRef.Par[i][4]->load();
        for (int x=0; x<8; ++x){
        voicenum[i][x]= oscindex[numv][x];
      }
    }

    for (int i=0; i<numSamples; ++i){

        attack = attack * atta;
        env = env * (attack+(deca*(1.0f-attack))); env = env * rele;
        env = std::max((sust*noteon), env);
        Env = env*(1.0f-attack);

        Mattack = Mattack * atta2;
        modenv = modenv * (Mattack+(deca2*(1.0f-Mattack))); modenv = modenv * rele2;
        modenv = std::max((sust2*noteon), modenv);
        MODenv = modenv*(1.0f-Mattack);

        float sound[4][2];

        vivdata+=viv;
        float bivr = wavetable[1][(vivdata >> 26)];
        float out[4][2];
        for (int x=0; x<4; ++x){
            sound[x][0]=0.0f;sound[x][1]=0.0f;
            float bivamo = 1.0f - (bivr*vivra[x]);
            int n = (x+5)%4;
            for (int y=0; y<8; ++y){
                float fmvalue = 1.0f - wavetable[waveforme[n]][(oscdata[n][0] >> 26)]*fm[x];
                oscdata[x][y]+=(uint32_t)((oscs[x][y] * fmvalue)*bivamo);
                sound[x][((y+2)%2)]+=wavetable[waveforme[x]][(oscdata[x][y] >> 26)]*voicenum[x][y];
                out[x][((y+2)%2)]=0.0f;
            }
        }
        float outmono[4];
        outmono[0]=0.0f;outmono[1]=0.0f;
        outmono[2]=0.0f;outmono[3]=0.0f;
        for (int x=0; x<4; ++x){
            for (int y=0; y<2; ++y){
                float gfix = gainfix[voicenum[x][y]];
                out[x][y] += (sound[x][y]*gfix)*volume[x];
                outmono[x] += (sound[x][y]*gfix)*volume[x];
            }
        }
        for (int x=0; x<4; ++x){
            for (int y=0; y<2; ++y){
                out[x][y] = (out[x][y]*stereo[x])+(outmono[x]*(1.0f-stereo[x]));
            }
        }
        float OuT[2];
        OuT[0]=out[0][0]+out[1][0]+out[2][0]+out[3][0];
        OuT[1]=out[0][1]+out[1][1]+out[2][1]+out[3][1];

        chL[i] += (OuT[0]*Env)*0.25f;
        chR[i] += (OuT[1]*Env)*0.25f;
    }
}
float atta=0.0f;float deca=0.0f;float sust=0.0f;float rele=0.0f;
float atta2=0.0f;float deca2=0.0f;float sust2=0.0f;float rele2=0.0f;

uint32_t oscs[4][8];

float modenv=0.0f;
float MODenv=0.0f;
float env=0.0f;
float Env=0.0f;
float Mattack=0.0f;
float attack=0.0f;

float Rate = 0.0f;

bool noteon =false;

uint32_t oscdata[4][8];

uint32_t viv=0;
uint32_t vivdata=0;

float oneHz = 0.0f;

    static inline constexpr float wavetable[4][64]{
    {-1.000000f,-0.968254f,-0.936508f,-0.904762f,-0.873016f,-0.841270f,-0.809524f,-0.777778f,
	-0.746032f,-0.714286f,-0.682540f,-0.650794f,-0.619048f,-0.587302f,-0.555556f,-0.523810f,
	-0.492063f,-0.460317f,-0.428571f,-0.396825f,-0.365079f,-0.333333f,-0.301587f,-0.269841f,
	-0.238095f,-0.206349f,-0.174603f,-0.142857f,-0.111111f,-0.079365f,-0.047619f,-0.015873f,
	0.015873f,0.047619f,0.079365f,0.111111f,0.142857f,0.174603f,0.206349f,0.238095f,
	0.269841f,0.301587f,0.333333f,0.365079f,0.396825f,0.428571f,0.460317f,0.492063f,
	0.523810f,0.555556f,0.587302f,0.619048f,0.650794f,0.682540f,0.714286f,0.746032f,
	0.777778f,0.809524f,0.841270f,0.873016f,0.904762f,0.936508f,0.968254f,1.000000f},

    {0.125000f,0.028884f,-0.068451f,-0.166165f,-0.263367f,-0.359119f,-0.452434f,-0.542276f,
	-0.627560f,-0.707153f,-0.779872f,-0.844487f,-0.899717f,-0.944234f,-0.976661f,-0.995572f,
	-0.999500f,-0.987897f,-0.962047f,-0.923400f,-0.873357f,-0.813272f,-0.744448f,-0.668141f,
	-0.585558f,-0.497856f,-0.406145f,-0.311485f,-0.214887f,-0.117315f,-0.019683f,0.077145f,
	0.172351f,0.265168f,0.354876f,0.440806f,0.522334f,0.598890f,0.669948f,0.735034f,
	0.793720f,0.845629f,0.890432f,0.927848f,0.957647f,0.979644f,0.993706f,0.999748f,
	0.997733f,0.987673f,0.969630f,0.943712f,0.910079f,0.868938f,0.820544f,0.765202f,
	0.703266f,0.635137f,0.561268f,0.482157f,0.398353f,0.310454f,0.219104f,0.125000f},

    {-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,
	-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,
	-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,
	-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,-1.000000f,
    1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,
	1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,
	1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,
	1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f,1.000000f},

    {0.900026f,0.518413f,0.118495f,-0.179627f,-0.400599f,-0.998290f,-0.798041f,-0.191308f,
	0.516807f,0.938326f,0.972879f,0.749369f,0.435679f,0.139972f,-0.095508f,-0.271748f,
	-0.431646f,-0.927161f,-0.943061f,-0.697409f,-0.269539f,0.220841f,0.638064f,0.901539f,
	0.998248f,0.959700f,0.833218f,0.662651f,0.480274f,0.306072f,0.150267f,0.016479f,
	-0.095687f,-0.188960f,-0.267637f,-0.337786f,-0.408945f,-0.499548f,-0.657126f,-0.999887f,
	-0.991328f,-0.965734f,-0.921029f,-0.858195f,-0.780355f,-0.691752f,-0.596896f,-0.499977f,
	-0.404528f,-0.313300f,-0.228267f,-0.150711f,-0.081347f,-0.020450f,0.032027f,0.076359f,
	0.112969f,0.142381f,0.165177f,0.181989f,0.193497f,0.200445f,0.203699f,0.204398f}
    };

    static inline const int oscindex[7][8]{
        {1,1,0,0,0,0,0,0},
        {0,0,1,1,0,0,0,0},
        {1,1,1,1,0,0,0,0},
        {0,0,1,1,1,1,0,0},
        {1,1,1,1,1,1,0,0},
        {0,0,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1}
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

AudioPluginAudioProcessor& processorRef;
};
//⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜⏝⏜
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       STATE (*this, nullptr, "NITRA", paral())
{
    oneHzuint = (uint32_t)((1 / getSampleRate())*4294967296.0f);
    
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    const juce::ScopedLock sl (getCallbackLock());
    synth.clearSounds();
    synth.clearVoices();
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const {return "MoldfairyX";}
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

double AudioPluginAudioProcessor::getTailLengthSeconds() const {return 0.0;}

int AudioPluginAudioProcessor::getNumPrograms() {return 1;}

int AudioPluginAudioProcessor::getCurrentProgram() {return 0;}

void AudioPluginAudioProcessor::setCurrentProgram (int index) {juce::ignoreUnused (index);}

const juce::String AudioPluginAudioProcessor::getProgramName (int index) {juce::ignoreUnused (index); return {};}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName) {juce::ignoreUnused (index, newName);}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto zero = [&](auto&& buf){
        std::fill(std::begin(buf), std::end(buf), 0.0f);
    };

    for (int i=0; i<4; ++i){
        for (int x=0; x<9; ++x){
            juce::String ids (oscpid[i][x]);
            Par[i][x]=STATE.getRawParameterValue(ids);
        }
    }

    oneHzuint = (uint32_t)((1 / getSampleRate())*4294967296.0f);

    onHZF = 1.0f / getSampleRate();

    pmt[0]=STATE.getRawParameterValue("biv");
    pmt[1]=STATE.getRawParameterValue("Fc");
    pmt[2]=STATE.getRawParameterValue("FcM");

    pmt[7]=STATE.getRawParameterValue("Fr");
    pmt[8]=STATE.getRawParameterValue("FrM");

    pmt[3]=STATE.getRawParameterValue("att");
    pmt[4]=STATE.getRawParameterValue("dec");
    pmt[5]=STATE.getRawParameterValue("sus");
    pmt[6]=STATE.getRawParameterValue("rel");

    pmt[9]=STATE.getRawParameterValue("DeF");
    pmt[10]=STATE.getRawParameterValue("DeV");
    pmt[11]=STATE.getRawParameterValue("ReF");
    pmt[12]=STATE.getRawParameterValue("ReD");
    pmt[13]=STATE.getRawParameterValue("ReV");

    pmt[14]=STATE.getRawParameterValue("DeL");
    pmt[15]=STATE.getRawParameterValue("DeR");

    pmt[16]=STATE.getRawParameterValue("ReHc");
    pmt[17]=STATE.getRawParameterValue("ReLc");

    pmt[18]=STATE.getRawParameterValue("matt");
    pmt[19]=STATE.getRawParameterValue("mdec");
    pmt[20]=STATE.getRawParameterValue("msus");
    pmt[21]=STATE.getRawParameterValue("mrel");

    pmt[22]=STATE.getRawParameterValue("cho");
    pmt[23]=STATE.getRawParameterValue("chs");
    pmt[24]=STATE.getRawParameterValue("chw");
    pmt[25]=STATE.getRawParameterValue("chd");

    pmt[26]=STATE.getRawParameterValue("wsm");
    pmt[27]=STATE.getRawParameterValue("wst");
    pmt[28]=STATE.getRawParameterValue("wsg");

    pmt[29]=STATE.getRawParameterValue("slo");
    pmt[30]=STATE.getRawParameterValue("ulo");

    pmt[31]=STATE.getRawParameterValue("sldp");
    pmt[32]=STATE.getRawParameterValue("uldp");
    pmt[33]=STATE.getRawParameterValue("slod");
    pmt[42]=STATE.getRawParameterValue("ulod");

    pmt[34]=STATE.getRawParameterValue("comp");
    
    pmt[35]=STATE.getRawParameterValue("test");

    pmt[37]=STATE.getRawParameterValue("compD");

    pmt[38]=STATE.getRawParameterValue("note1");
     pmt[39]=STATE.getRawParameterValue("note2");
      pmt[40]=STATE.getRawParameterValue("note3");
       pmt[41]=STATE.getRawParameterValue("note4");

    for (int i=0;i<8;++i){
        zero(combsL[i]);
        zero(combsR[i]);
        zero(reverbHPFL[i]);
        zero(reverbHPFR[i]);
    }

    zero(reverbFil[0]);
    zero(reverbFil[1]);
    zero(chorusbuf[0]);
    zero(chorusbuf[1]);

    for (int i=0;i<6;++i){
        zero(apfsL[i]);zero(apfsR[i]);
         zero(apfsL2[i]);zero(apfsR2[i]);
      
    }

    zero(delay[0]);zero(delay[1]);

    zero(filterap[0][0]);
    zero(filterap[1][1]);
    zero(filterap[0][2]);
    zero(filterap[1][3]);

    zero(random);

    writeindex =0;
    writeindex2 =0;
    writeindex3 =0;

    synth.setCurrentPlaybackSampleRate(sampleRate);

    voice(4);

    Srate=(float)getSampleRate();

    //for(uint_fast16_t r=0;r<128;++r){
        //random[r]=getrandom()*2.3283064e-10f;
    //}

    for(uint_fast16_t r=0;r<3;++r){
        comp[0][r].reset();
        comp[1][r].reset();
    }
}

void AudioPluginAudioProcessor::releaseResources()
{
    voice(4);
    Srate=(float)getSampleRate();
   
    for(uint_fast16_t r=0;r<3;++r){
        comp[0][r].reset();
        comp[1][r].reset();
    }

    oneHzuint = (uint32_t)((1 / getSampleRate())*4294967296.0f);
    onHZF = 1.0f / getSampleRate();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float sample = buffer.getNumSamples();

    auto* chL = buffer.getWritePointer (0);
    auto* chR = buffer.getNumChannels() > 1 ? buffer.getWritePointer (1) : chL;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, sample);

    synth.renderNextBlock(buffer, midiMessages, 0, sample);
    uint32_t culp=0;
    if (auto* playHead = getPlayHead()){
        if (auto posi = playHead->getPosition()){
            if (posi->getBpm().hasValue()){
                BPM = static_cast<float>(*(posi->getBpm()));
            }
            if (posi->getPpqPosition().hasValue()){
                uint64_t culpl = static_cast<uint64_t>(*posi->getPpqPosition() * 268435456);
                culp = static_cast<uint32_t>(culpl);
            }
            if (posi->getIsPlaying()){
                plaing=true;
            }else{plaing=false;}
        }
    }

    uint_fast8_t pqp = (culp>>31);
    
    float numdf = std::min(1023.0f,std::max(0.0f, (BPM - 1)));
    int_fast16_t numd = (int_fast16_t)numdf;
    float lined = numdf - numd;
    float delayS = (bpms[numd]*(1.0f-lined))+(bpms[std::min(1023, (numd+1))]*lined);
    float delayhz = (bpmshz[numd]*(1.0f-lined))+(bpmshz[std::min(1023, (numd+1))]*lined);

    delayphase = (uint32_t)(oneHzuint*delayhz);
    int delaynumsample = static_cast<int>(delayS * Srate);

    int DeLs = pmt[14]->load()*delaynumsample;
    int DeRs = pmt[15]->load()*delaynumsample;

    float delayfb = pmt[9]->load();
    float delayVo = pmt[10]->load();
    float reverbfb = pmt[11]->load();
    float reverbDf = pmt[12]->load()*0.5f;
    float reverbVo = pmt[13]->load();

    float reverbHc = pmt[16]->load()*0.6f;
    float reverbLc = 0.001f+pmt[17]->load()*0.01f;

    float chorusamo = pmt[22]->load();
    float chorusspd = pmt[23]->load()*10.0f;
    float choruswid = pmt[24]->load()*0.5f;
    float chorusdep = pmt[25]->load()*400.0f;
    float wsmix = pmt[26]->load();
    float wstype = pmt[27]->load();
    float wsgain = pmt[28]->load();
    uint_fast8_t slfor = 29 - pmt[29]->load();
    float uslfor = pmt[30]->load();

    float ULDP = pmt[31]->load();
    float SLDP = pmt[32]->load();
    uint_fast8_t ULDS = (uint_fast8_t)pmt[33]->load();
    uint_fast8_t SLDS = (uint_fast8_t)pmt[42]->load();
   
    float compmix = pmt[34]->load();
    float testparam = pmt[35]->load();
    float compoutg = pmt[37]->load();
    
    for (uint_fast16_t i=0; i<sample; ++i){
        ulfo +=(uint32_t)(delayphase*0.015625f);
        lfo+=(uint32_t)(delayphase*uslfor);
        
        float LFO2 = lfo*2.3283064e-10f;
        LFO2=std::min(LFO2, (1.0f-LFO2));
        LFO2=-1.0f+(LFO2*4.0f);

        LFO2*=SLDP;

        float noize = -1.0f+(getrandom()*2.3283064e-10f)*2.0f;

        float wnoize = noize *0.08f;

        uint32_t delx = (writeindex2 - DeLs)&1048575;
        uint32_t derx = (writeindex2 - DeRs)&1048575;
        
        float modENV = 0.0f;
        float envv = 0.0f;
        for (int_fast8_t v=0; v<synth.getNumVoices(); ++v){
            if (auto env = dynamic_cast<Wave*>(synth.getVoice(v))){
                modENV = std::max(modENV, (env->MODenv));
                envv = std::max(envv, (env->Env));
            }
        }
        uint_fast32_t liplfo = (uint_fast32_t)((culp*plaing)+(ulfo*(1-plaing)));
        uint_fast16_t idxlfo = (uint_fast16_t)((liplfo>>slfor)&127);
        float rfol = ((liplfo&uints[(slfor-1)])*uinttofloat[(slfor-1)]);
        
        randomlfo = random[idxlfo]*ULDP;

        random[(idxlfo+1)&127]=noize;

        float LFOmod[3][2];

        LFOmod[0][0]=0.0f;
        LFOmod[0][1]=0.0f;
        LFOmod[1][0]=1.0f;
        LFOmod[1][1]=0.0f;
        LFOmod[2][0]=0.0f;
        LFOmod[2][1]=1.0f;

        float reso = pmt[7]->load()+(modENV*(pmt[8]->load()))+((randomlfo*LFOmod[ULDS][1]))+(LFO2*LFOmod[SLDS][1]);
       float cut = pmt[1]->load()+(modENV*(pmt[2]->load()))+((randomlfo*LFOmod[ULDS][0]))+(LFO2*LFOmod[SLDS][0]);
       cut = std::max(0.0f, (std::min(cut, 1.0f))); reso = std::max(0.0f,(std::min(reso, 1.0f)));

        reso*=2.0f;

        reso=std::max(reso, cut);

        float noizeOSC =(noize*envv)*testparam;

        chL[i]+=noizeOSC;
        chR[i]+=noizeOSC;

        float clL=svF[0].process24(chL[i],cut,(2.0f-reso),1.0f,0.0f,0.0f);
        float clR=svF[1].process24(chR[i],cut,(2.0f-reso),1.0f,0.0f,0.0f);

        clL=wshapeing(clL, wsmix, wsgain, wstype);
        clR=wshapeing(clR, wsmix, wsgain, wstype);

        chorusdata+= (uint32_t)(oneHzuint*chorusspd);
      
        uint32_t chorusdatacos = (uint32_t)(chorusdata+1073741824);

        float chw = chorusdata*2.3283064e-10f;
        chw=std::min(chw, (1.0f-chw));
        chw*=2.0f;
        float chw2 = chorusdatacos*2.3283064e-10f;
        chw2=std::min(chw2, (1.0f-chw2));
        chw2*=2.0f;

        float chrL = chorus(chw, clL, chorusdep, chorusbuf[0], writeindex3, 0);
        float chrR = chorus(chw, clR, chorusdep, chorusbuf[1], writeindex3, 1);
        float chrLm = chorus(chw2, clL, chorusdep, chorusbuf[0], writeindex3, 0);
        float chrRm = chorus(chw2, clR, chorusdep, chorusbuf[1], writeindex3, 0);

        float chorsampL =((chrLm*(1.0f-choruswid))+chrL*choruswid);
        float chorsampR =((chrRm*(1.0f-choruswid))+chrR*choruswid);

        chorsampL=chorsampL+(wnoize*(std::fabsf(chorsampL)));
        chorsampR=chorsampR+(wnoize*(std::fabsf(chorsampR)));

        clL+=chorsampL*chorusamo;
        clR+=chorsampR*chorusamo;
        
        float Ldelay = delay[0][delx];
        float Rdelay = delay[1][derx];

        delay[0][writeindex2]=clL+(Ldelay*delayfb);
        delay[1][writeindex2]=clR+(Rdelay*delayfb);

        chL[i]=clL+(Ldelay*delayVo);
        chR[i]=clR+(Rdelay*delayVo);

        uint_fast16_t apf[2][6];
        for (int_fast8_t y=0;y<6;++y){
            apf[0][y]=(uint_fast16_t)(writeindex3 - apfindex[0][y])&1023;
            apf[1][y]=(uint_fast16_t)(writeindex3 - apfindex[1][y])&1023;
        }

        float apfdLv[6];float apfdRv[6];
        apfdLv[0] = APF(chL[i], reverbDf, apfsL[0], apfsL2[0], writeindex3, apf[0][0]);
        apfdRv[0] = APF(chR[i], reverbDf, apfsR[0], apfsR2[0], writeindex3, apf[1][0]);

        for (int_fast8_t z=0;z<5;++z){
            int_fast8_t n = z+1;
            apfdLv[n] = APF(apfdLv[z], reverbDf, apfsL[n], apfsL2[n], writeindex3, apf[0][n]);
            apfdRv[n] = APF(apfdRv[z], reverbDf, apfsR[n], apfsR2[n], writeindex3, apf[1][n]);
        }

        float combsval[16];
        for (int_fast8_t a=0; a<8; ++a){
            uint_fast16_t idxc1 = (writeindex - combindex[0][a])&4095;
            uint_fast16_t idxc2 = (writeindex - combindex[1][a])&4095;
            float foutL = oneHPF(oneIIR(combsL[a][idxc1], reverbFil[0][a], reverbHc), reverbHPFL[a], reverbLc);
            float foutR = oneHPF(oneIIR(combsR[a][idxc2], reverbFil[1][a], reverbHc), reverbHPFR[a], reverbLc);
            combsval[a] = foutL;
            combsval[(15-a)] = foutR;
        }

        Hadamard16(combsval);
        for (int_fast8_t b=0; b<8; ++b){
        combsL[b][writeindex]=(apfdLv[5]*(1.0f-(reverbfb*0.4f)))+combsval[(15-b)]*reverbfb;
        combsR[b][writeindex]=(apfdRv[5]*(1.0f-(reverbfb*0.4f)))+combsval[b]*reverbfb;
        }

        float fdnoutL=0.0f;float fdnoutR=0.0f;
        for (int_fast8_t b=0; b<16; b+=2){
            fdnoutL+=combsval[b];
            fdnoutR+=combsval[(b+1)];
        }
        
        float culL = (chL[i]*(1.0f-reverbVo))+(fdnoutL*0.75f)*reverbVo;
        float culR = (chR[i]*(1.0f-reverbVo))+(fdnoutR*0.75f)*reverbVo;

        float trband[2][4];

        trband[0][0] = band[0].process3band(culL, 0);
        trband[0][1] = band[0].midband;
        trband[0][2] = band[0].highband;

        trband[1][0] = band[1].process3band(culR, 0);
        trband[1][1] = band[1].midband;
        trband[1][2] = band[1].highband;

        float drysigL = (trband[0][0]+trband[0][1]+trband[0][2]);
        float drysigR = (trband[1][0]+trband[1][1]+trband[1][2]);

            float gaininp[3];
            gaininp[0] = 1.71f;
            gaininp[1] = 1.0f;
            gaininp[2] = 1.87f;

        float outcompL=0.0f;float outcompR=0.0f;

            for (uint_fast8_t o=0;o<3;++o){
                outcompL+=comp[0][o].process((trband[0][o]*gaininp[o]), compprams[o][0],compprams[o][1],compprams[o][2],compprams[o][3]);
                outcompR+=comp[1][o].process((trband[1][o]*gaininp[o]), compprams[o][0],compprams[o][1],compprams[o][2],compprams[o][3]);
            }

            outcompL*=compoutg;outcompR*=compoutg;

            bool compbool = static_cast<bool>(compmix);

            drysigL = (culL*(1.0f-compbool))+drysigL*compbool;
            drysigR = (culR*(1.0f-compbool))+drysigR*compbool;

            chL[i]=(drysigL*(1.0f-compmix))+(outcompL*compmix);
            chR[i]=(drysigR*(1.0f-compmix))+(outcompR*compmix);

            Lvol=(Lvol*0.99f)+std::fabsf(chL[i])*0.01f;
            Rvol=(Rvol*0.99f)+std::fabsf(chR[i])*0.01f;

        firidx=(firidx+1)&15;
        writeindex = (writeindex+1)&4095;
        writeindex2 = (writeindex2+1)&1048575;
        writeindex3 = (writeindex3+1)&1023;
    }
    midiMessages.clear();
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::XmlElement xml ("mrfd");
    auto State = STATE.copyState();
    std::unique_ptr<juce::XmlElement>apvtsXml (State.createXml());
    xml.addChildElement (apvtsXml.release());
    copyXmlToBinary (xml, destData);  
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlStates (getXmlFromBinary (data, sizeInBytes));
    if (xmlStates != nullptr){
        if (xmlStates->hasTagName("mrfd")){
            auto apS = xmlStates->getChildByName(STATE.state.getType().toString());
            if (apS != nullptr){
                STATE.replaceState(juce::ValueTree::fromXml(*apS));
                updateHostDisplay();
            }
        }
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

inline void AudioPluginAudioProcessor::Hadamard16 (float* Hamadard)
{
    for (int_fast8_t i = 0; i < 16; i += 2) {
        float a = Hamadard[i], b = Hamadard[i+1];
        Hamadard[i] = a + b; Hamadard[i+1] = a - b;
    }
    for (int_fast8_t i = 0; i < 16; i += 4) {
        for (int_fast8_t j = 0; j < 2; ++j) {
            float a = Hamadard[i+j], b = Hamadard[i+j+2];
            Hamadard[i+j] = a + b; Hamadard[i+j+2] = a - b;
        }
    }
    for (int_fast8_t i = 0; i < 16; i += 8) {
        for (int_fast8_t j = 0; j < 4; ++j) {
            float a = Hamadard[i+j], b = Hamadard[i+j+4];
            Hamadard[i+j] = a + b; Hamadard[i+j+4] = a - b;
        }
    }
    for (int_fast8_t j = 0; j < 8; ++j) {
        float a = Hamadard[j], b = Hamadard[j+8];
        Hamadard[j] = a + b; Hamadard[j+8] = a - b;
    }
    for (int_fast8_t i = 0; i < 16; ++i) {
        Hamadard[i] *= 0.25f;
    }
}

inline void AudioPluginAudioProcessor::Hadamard8 (float* Hamadard)
{
    for (int_fast8_t i = 0; i < 8; i += 2) {
        float a = Hamadard[i], b = Hamadard[i+1];
        Hamadard[i] = a + b; Hamadard[i+1] = a - b;
    }
    for (int_fast8_t i = 0; i < 8; i += 4) {
        for (int_fast8_t j = 0; j < 2; ++j) {
            float a = Hamadard[i+j], b = Hamadard[i+j+2];
            Hamadard[i+j] = a + b; Hamadard[i+j+2] = a - b;
        }
    }
    for (int_fast8_t i = 0; i < 4; ++i) {
        float a = Hamadard[i], b = Hamadard[i+4];
        Hamadard[i] = a + b; Hamadard[i+4] = a - b;
    }
    
    for (int_fast8_t i = 0; i < 8; ++i) {
        Hamadard[i] *= 0.25f;
    }
}
inline void AudioPluginAudioProcessor::voice (int num)
 {
    synth.clearVoices();
    synth.clearSounds();
    
    for (uint_fast8_t i=0; i<num; ++i){
        synth.addVoice(new Wave(*this));
    }
    synth.addSound(new Synth());
 }

 inline float AudioPluginAudioProcessor::APF (float in, float d, float* buf2, float* buf, int x, int r)
 {
    buf2[x] = (in*-d)+buf[r]+(buf2[r]*d);
    buf[x]=in;
    return buf2[x];
 }

 inline float AudioPluginAudioProcessor::oneIIR (float in, float& b, float r)
 {
    b = (in * (1.0f-r))+b*r;
    return b;
 }

 inline float AudioPluginAudioProcessor::oneHPF (float in, float* b, float R)
 {
    float r = 1.0f-R;
    float out = r*(b[0]+in-b[1]);
    b[0]=out;
    b[1]=in;
    return out;
 }
 inline float AudioPluginAudioProcessor::chorus (float ix, float in, float d, float* b, int x, int p)
 {
    b[x]=in;
    float t = (ix*p)+((1.0f-ix)*(1.0f-p));
    uint_fast16_t n = (uint_fast16_t)(t*d);
    float out = b[(uint_fast16_t)(x-n)&1023];
    return out;
 }

 inline float AudioPluginAudioProcessor::wshapeing (float in, float m, float g, float t)
 {
    float x = std::max(0.1f, (1.0f - (std::fabsf(in * (1.0f+(g*2.0f))))));
        x*=x;
    float y = (in * x)*(1.5f+g);
    float z = (in * (x*x*x*x))*(5.0f+g);
    float o = (y*(1.0f-t))+z*t;
    return (in*(1.0f-m))+o*m;
 }
 inline uint32_t AudioPluginAudioProcessor::getrandom ()
 {
    xorradom ^= xorradom << 15;
    xorradom ^= xorradom >> 13;
    xorradom ^= xorradom << 7;
    return xorradom;
 }