/*
  ==============================================================================

    LfoEnabledValue.h
    Created: 4 Dec 2023 5:12:37pm
    Author:  Ari Troper

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOData.h"

class LfoEnabledValue {
public:
    // Constructor
    LfoEnabledValue(LFOData& lfo, double value, double min, double max, double depth = 0.0);

    // Getter and Setter for value
    double getLfodValue() const;
    void setValue(double value);
    
    // Getter and Setter for depth
    double getDepth() const;
    void setDepth(double depth);

private:
    LFOData& lfo_;
    double min_; // The minimum value the LFO can set
    double max_; // The maximum value the LFO can set
    double depth_;  // Depth of the LFO modulation
    double value_;  // Actual value of the slider with LFO applied
};
