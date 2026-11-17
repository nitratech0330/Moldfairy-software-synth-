#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "JuceHeader.h"

#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 1

#include <string.h>

#include <algorithm>

#include <cmath>

#include <vector>

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::File file = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile);
    for (int i=0; i<7; ++i){
        files[i]=file.getChildFile("Contents")
                    .getChildFile("Resources")
                    .getChildFile("images")
                    .getChildFile(IMGname[i]);
        if (files[i].existsAsFile()){
            IMGs[i]=juce::ImageFileFormat::loadFrom(files[i]);
        }
    }
    std::vector<std::pair<juce::String, juce::Slider*>> slidermap{
            {"Fc", &sliders2[8]},
            {"Fr", &sliders2[9]},
            {"FcM", &sliders2[10]},
            {"FrM", &sliders2[11]},

            {"note1", &sliders2[12]},
            {"note2", &sliders2[13]},
            {"note3", &sliders2[14]},
            {"note4", &sliders2[15]},

            {"wsm", &sliders2[16]},
            {"wst", &sliders2[17]},
            {"wsg", &sliders2[18]},

            {"cho", &sliders2[19]},
            {"chs", &sliders2[20]},
            {"chd", &sliders2[21]},
            {"chw", &sliders2[22]},

            {"DeV", &sliders2[23]},
            {"DeF", &sliders2[24]},
            {"DeL", &sliders2[25]},
            {"DeR", &sliders2[26]},

            {"ReV", &sliders2[27]},
            {"ReF", &sliders2[28]},
            {"ReD", &sliders2[29]},
            {"ReLc", &sliders2[30]},
            {"ReHc", &sliders2[31]},

            {"sldp", &sliders2[32]},
            {"uldp", &sliders2[33]},
            {"slo", &sliders2[34]},
            {"ulo", &sliders2[35]},

            {"test", &sliders2[36]},

            {"compD", &sliders2[37]},
            {"comp", &sliders2[38]},
            {"biv", &sliders2[39]}
        };
        
        for (int i=0;i<4;++i){
            for (int x=0;x<9;++x){
            slidermap.push_back({processorRef.oscpid[i][x], &oscsliders[i][x]});
            }
            
        }
    std::vector<std::pair<juce::String, juce::Slider*>> slidermap2{
            {"att", &sliders2[0]},{"dec", &sliders2[1]},{"sus", &sliders2[2]},{"rel", &sliders2[3]},
            {"matt", &sliders2[4]},{"mdec", &sliders2[5]},{"msus", &sliders2[6]},{"mrel", &sliders2[7]}
        };

    for (const auto& [paramID, sliderPtr] : slidermap){
        auto b = sliderPtr->getBounds();
        addAndMakeVisible(sliderPtr);
        sliderPtr->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        sliderPtr->setAlpha(0.0);
        sliderPtr->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        sliderPtr->onValueChange = [this, b](){repaint(b.getX(),b.getY(),b.getWidth(),b.getHeight());};
        attach.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.STATE, paramID, *sliderPtr));
        sliders.push_back(sliderPtr);
    };

    for (const auto& [paramID, sliderPtr] : slidermap2){
        auto b = sliderPtr->getBounds();
        addAndMakeVisible(sliderPtr);
        sliderPtr->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        sliderPtr->setAlpha(0.0);
        sliderPtr->setSliderStyle (juce::Slider::LinearVertical);
        sliderPtr->onValueChange = [this, b](){repaint(b.getX(),b.getY(),b.getWidth(),b.getHeight());};
        attach.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.STATE, paramID, *sliderPtr));
        sliders2v.push_back(sliderPtr);
    };

    addAndMakeVisible(B);
    B.onClick = [this](){showmenu();};
    B.setAlpha(0.0f);

    addAndMakeVisible(B2);
    B2.onClick = [this](){showmenu2();};
    B2.setAlpha(0.0f);

    sliders2[25].onValueChange = [this](){auto Db = sliders2[26].getBounds(); repaint(Db.getX()+50, Db.getY()+6, 60, 15);};
    sliders2[26].onValueChange = [this](){auto Db = sliders2[26].getBounds(); repaint(Db.getX()+50, Db.getY()+20, 60, 15);};

    setSize (800, 400);
    startTimerHz(15);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (IMGs[6].isValid()){
        g.drawImage(IMGs[6],0,0,800,400,0,0,800,400, false);
    }
    
    if (IMGs[0].isValid()){
        for (const auto& s : sliders){
            float n=s->valueToProportionOfLength(s->getValue());
            int num = juce::jlimit(0, 100, (int)(n * 100));
            int x = 60*((num+4) / 6);
            int y = 60*((num+4) % 6);
            auto b = s->getBounds();
            g.drawImage(IMGs[0], b.getX(), b.getY(), b.getWidth(), b.getHeight(), x, y, 60, 60, false);
        }
    }

    if (IMGs[1].isValid()){
        for (const auto& s : sliders2v){
            float n=s->valueToProportionOfLength(s->getValue());
            int_fast8_t num = juce::jlimit(0, 75, (int)(n * 75));
            auto b = s->getBounds();
            g.drawImage(IMGs[1], b.getX(), b.getY()+(75-num), 32, 32, 0, 0, 32, 32, false);
        }
    }

    if (IMGs[2].isValid()){
        float LUV = processorRef.Lvol;
        LUV=1.0f-LUV;
        LUV=LUV*LUV*LUV*LUV;
        LUV=1.0f-LUV;
        float RUV = processorRef.Rvol;
        RUV=1.0f-RUV;
        RUV=RUV*RUV*RUV*RUV;
        RUV=1.0f-RUV;
        int_fast8_t VUNL = LUV*45.0f;
        int_fast8_t VUNR = RUV*45.0f;
        g.drawImage(IMGs[2], 625, 350, 70, 35, 0, VUNL*100, 200, 100, false);
        g.drawImage(IMGs[2], 700, 350, 70, 35, 0, VUNR*100, 200, 100, false);
    }

    if (IMGs[3].isValid()){
        auto Rb = sliders2[26].getBounds();
        g.drawImage(IMGs[3],Rb.getX()+50, Rb.getY()+6, 60, 15, 0, (int_fast16_t)(320+sliders2[25].getValue()*20), 300, 20, false);
        g.drawImage(IMGs[3],Rb.getX()+50, Rb.getY()+20, 60, 15, 0, (int_fast16_t)(sliders2[26].getValue()*20), 300, 20, false);
    }

    if (IMGs[5].isValid()){
        auto b = B.getBounds();
        auto b2 = B2.getBounds();
        g.drawImage(IMGs[5], b.getX(), b.getY(), b.getWidth(), b.getHeight(), 0, (menunum*40), 120, 40, false);
        g.drawImage(IMGs[5], b2.getX(), b2.getY(), b2.getWidth(), b2.getHeight(), 0, (menunum2*40), 120, 40, false);
    }
    
}

void AudioPluginAudioProcessorEditor::resized()
{
    oscsliders[0][1].setBounds(34, 25, 20, 20);
    oscsliders[0][2].setBounds(28, 47, 25, 25);
    oscsliders[0][0].setBounds(50, 25, 50, 50);
    oscsliders[0][3].setBounds(100, 25, 50, 50);
    oscsliders[0][6].setBounds(150, 25, 50, 50);
    oscsliders[0][7].setBounds(200, 25, 50, 50);
    oscsliders[0][8].setBounds(250, 25, 50, 50);

    oscsliders[0][5].setBounds(302, 50, 20, 20);
    oscsliders[0][4].setBounds(300, 25, 25, 25);

    //oscsliders[0][5].setBounds(100, 70, 50, 50);

    oscsliders[1][1].setBounds(484, 25, 20, 20);
    oscsliders[1][2].setBounds(478, 47, 25, 25);
    oscsliders[1][0].setBounds(500, 25, 50, 50);
    oscsliders[1][3].setBounds(550, 25, 50, 50);
    oscsliders[1][6].setBounds(600, 25, 50, 50);
    oscsliders[1][7].setBounds(650, 25, 50, 50);
    oscsliders[1][8].setBounds(700, 25, 50, 50);

    oscsliders[1][5].setBounds(754, 51, 20, 20);
    oscsliders[1][4].setBounds(752, 26, 25, 25);

    oscsliders[2][1].setBounds(34, 120, 20, 20);
    oscsliders[2][2].setBounds(28, 142, 25, 25);
    oscsliders[2][0].setBounds(50, 120, 50, 50);
    oscsliders[2][3].setBounds(100, 120, 50, 50);
    oscsliders[2][6].setBounds(150, 120, 50, 50);
    oscsliders[2][7].setBounds(200, 120, 50, 50);
    oscsliders[2][8].setBounds(250, 120, 50, 50);

    oscsliders[2][5].setBounds(302, 145, 20, 20);
    oscsliders[2][4].setBounds(300, 120, 25, 25);

    oscsliders[3][1].setBounds(484, 120, 20, 20);
    oscsliders[3][2].setBounds(478, 142, 25, 25);
    oscsliders[3][0].setBounds(500, 120, 50, 50);
    oscsliders[3][3].setBounds(550, 120, 50, 50);
    oscsliders[3][6].setBounds(600, 120, 50, 50);
    oscsliders[3][7].setBounds(650, 120, 50, 50);
    oscsliders[3][8].setBounds(700, 120, 50, 50);

    oscsliders[3][5].setBounds(754, 146, 20, 20);
    oscsliders[3][4].setBounds(752, 121, 25, 25);

    sliders2[0].setBounds(333, 35, 32, 100);
    sliders2[1].setBounds(369, 35, 32, 100);
    sliders2[2].setBounds(400, 35, 32, 100);
    sliders2[3].setBounds(435, 35, 32, 100);

    sliders2[4].setBounds(333, 155, 32, 100);
    sliders2[5].setBounds(369, 155, 32, 100);
    sliders2[6].setBounds(400, 155, 32, 100);
    sliders2[7].setBounds(435, 155, 32, 100);

    sliders2[8].setBounds(40, 195, 60, 60);
    sliders2[9].setBounds(110, 195, 60, 60);
    sliders2[10].setBounds(190, 200, 40, 40);
    sliders2[11].setBounds(240, 200, 40, 40);

    sliders2[12].setBounds(15, 30, 20, 20);
    sliders2[13].setBounds(465, 30, 20, 20);
    sliders2[14].setBounds(15, 130, 20, 20);
    sliders2[15].setBounds(465, 130, 20, 20);

    sliders2[16].setBounds(502, 210, 40, 40);
    sliders2[17].setBounds(552, 210, 40, 40);
    sliders2[18].setBounds(602, 210, 40, 40);
    sliders2[19].setBounds(552, 270, 40, 40);
    sliders2[20].setBounds(602, 270, 40, 40);
    sliders2[21].setBounds(652, 270, 40, 40);
    sliders2[22].setBounds(702, 270, 40, 40);

    sliders2[23].setBounds(40, 270, 40, 40);
    sliders2[24].setBounds(90, 270, 40, 40);
    sliders2[25].setBounds(140, 270, 40, 40);
    sliders2[26].setBounds(190, 270, 40, 40);

    sliders2[27].setBounds(50, 325, 40, 40);
    sliders2[28].setBounds(100, 325, 40, 40);
    sliders2[29].setBounds(150, 325, 40, 40);
    sliders2[30].setBounds(200, 335, 30, 30);
    sliders2[31].setBounds(230, 335, 30, 30);

    B.setBounds(325, 295, 60, 20);

    B2.setBounds(325, 340, 60, 20);

    sliders2[32].setBounds(395, 285, 40, 40);
    sliders2[33].setBounds(395, 330, 40, 40);

    sliders2[34].setBounds(440, 290, 30, 30);
    sliders2[35].setBounds(440, 335, 30, 30);

    sliders2[36].setBounds(675, 180, 50, 50);

    sliders2[37].setBounds(585, 350, 30, 30);
    sliders2[38].setBounds(535, 340, 45, 45);

    sliders2[39].setBounds(732, 203, 30, 30);


    //oscsliders[0][8].setBounds(250, 25, 50, 50);


}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    repaint(625, 350, 145, 35);

}
void AudioPluginAudioProcessorEditor::showmenu()
{
    juce::PopupMenu menu;
    menu.addItem(1, "NULL");
    menu.addItem(2, "CUTOFF");
    menu.addItem(3, "RESONANCE");
    menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&B),[this](int d){
        if (d == 0) return;
        auto* p = processorRef.STATE.getParameter("slod");
        float n = p->convertTo0to1((float)(d-1.0f));
        p->setValueNotifyingHost(n);
        menunum=(d-1.0f);
        auto b = B.getBounds();
        repaint(b.getX(), b.getY(), b.getWidth(), b.getHeight());
    });
}

void AudioPluginAudioProcessorEditor::showmenu2()
{
    juce::PopupMenu menu;
    menu.addItem(1, "NULL");
    menu.addItem(2, "CUTOFF");
    menu.addItem(3, "RESONANCE");
    menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&B2),[this](int d){
        if (d == 0) return;
        auto* p = processorRef.STATE.getParameter("ulod");
        float n = p->convertTo0to1((float)(d-1.0f));
        p->setValueNotifyingHost(n);
        menunum2=(d-1.0f);
        auto b = B2.getBounds();
        repaint(b.getX(), b.getY(), b.getWidth(), b.getHeight());
    });
}


