/*
  ==============================================================================

    ADSRData.cpp
    Created: 23 Nov 2023 10:40:20pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "ADSRData.h"

void AdsrData::setAdsrParameters (const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    setParameters(adsrParams);
}
