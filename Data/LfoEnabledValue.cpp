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

// Getter and Setter for value
double LfoEnabledValue::getLfodValue() const {
    // Calculate the modulation amount based on the depth
    double modulationAmount = value_ * depth_ * (max_ - min_);

    // Apply modulation between minValue and maxValue
    double modulatedValue = value_ + modulationAmount * lfo_.mod;

    // Ensure the modulatedValue stays within the specified range
    modulatedValue = std::min(std::max(modulatedValue, min_), max_);
    
    std::cout << modulatedValue;
    
    return value_;
}

void LfoEnabledValue::setValue(double value) {
    value_ = value;
}

void LfoEnabledValue::setDepth(double depth) {
    depth_ = depth;
}

double LfoEnabledValue::getDepth() const {
    return depth_;
}
