#pragma once

#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 1

#include "PluginProcessor.h"

#include <array>

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void showmenu();
    void showmenu2();
    void showPresetmenu();
    void showPresetSavemenu();
    void setdef();

    uint_fast8_t menunum=0;
    uint_fast8_t menunum2=0;

    std::array<juce::File, 16>files;
    std::array<juce::Image, 16>IMGs;
    std::array<juce::StringRef, 16>IMGname={{
        "img0.png","img1.png","img2.png","img3.png",
        "img4.png","img5.png","img6.png","img7.png",
        "img8.png","img9.png","img10.png","img11.png",
        "img12.png","img13.png","img14.png","img15.png"
    }};

    juce::File def;
    juce::TextButton B;
    juce::TextButton B2;
    juce::TextButton B3;

    std::unique_ptr<juce::FileChooser> Choose;
    
    std::array<std::array<juce::Slider, 9>,4>oscsliders;
    std::array<juce::Slider, 48>sliders2;
    std::vector<juce::Slider*> sliders2v;
    std::vector<juce::Slider*> sliders;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
