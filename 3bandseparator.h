#pragma once
#include <cmath>
#include <algorithm>

class threebandseparator
{
public:
    
threebandseparator()
{
    Lowband=0.0f;
    midband=0.0f;
    highband=0.0f;
    
    std::fill(std::begin(h),std::end(h),0.0f);
    std::fill(std::begin(b),std::end(b),0.0f);
    std::fill(std::begin(l),std::end(l),0.0f);
}
~threebandseparator(){}

inline float process3band(float in, int type)
{
    auto svf = [](float IN, float C, float& Low, float& Band, float& High){
        High = IN - Low - 1.45f * Band;
        Band = C * High + Band;
        Low = Low + C * Band;
    };

    svf(in, 0.0142471038f, l[0], b[0], h[0]);
    svf(l[0], 0.0142471038f, l[1], b[1], h[1]);
    svf(h[0], 0.0142471038f, l[2], b[2], h[2]);

    Lowband=l[1];

    svf(h[2], 0.1911586287f, l[3], b[3], h[3]);
    svf(l[3], 0.1911586287f, l[4], b[4], h[4]);
    svf(h[3], 0.1911586287f, l[5], b[5], h[5]);

    midband=l[4]; highband=h[5]*0.75f;

    float t[3];

    t[0]=Lowband;
    t[1]=midband;
    t[2]=highband;

    return t[type];
}
    float Lowband=0.0f;
    float midband=0.0f;
    float highband=0.0f;

private:

    float l[6];float b[6];float h[6];

};