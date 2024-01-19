/*
  ==============================================================================

    LfoEnabledValue.cpp
    Created: 4 Dec 2023 5:12:37pm
    Author:  Ari Troper

  ==============================================================================
*/

#include "LfoEnabledValue.h"
#include "LFOData.h"

// Constructor
LfoEnabledValue::LfoEnabledValue(LFOData& lfo, double value, double min, double max, double depth) : lfo_(lfo), min_(min), max_(max), depth_(depth), value_(value) { }

double LfoEnabledValue::getLfodValue() const {
    // Calculate the modulation amount based on the depth and LFO modulation
    double modulationAmount = (depth_ / 100) * (max_ - min_) * lfo_.mod;

    // Apply modulation to the value
    double modulatedValue = value_ + modulationAmount;

    // Ensure the modulatedValue stays within the specified range
    modulatedValue = std::min(std::max(modulatedValue, min_), max_);
    
    return modulatedValue;
}

void LfoEnabledValue::setValue(double value) {
    value_ = value;
}

void LfoEnabledValue::setDepth(double depth) {
    depth_ = std::min(std::max(depth, 0.0), 100.0);
}

double LfoEnabledValue::getDepth() const {
    return depth_;
}
