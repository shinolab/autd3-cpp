#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct STMSamplingModeFreq {
  Freq<double> freq;

  native_methods::FocusSTMPtr focus_ptr() const { return native_methods::AUTDSTMFocusFromFreq(freq.hz()); }
  native_methods::GainSTMPtr gain_ptr() const { return native_methods::AUTDSTMGainFromFreq(freq.hz()); }
};

struct STMSamplingModeFreqNearest {
  Freq<double> freq;

  native_methods::FocusSTMPtr focus_ptr() const { return native_methods::AUTDSTMFocusFromFreqNearest(freq.hz()); }
  native_methods::GainSTMPtr gain_ptr() const { return native_methods::AUTDSTMGainFromFreqNearest(freq.hz()); }
};

struct STMSamplingModeSamplingConfig {
  native_methods::SamplingConfigWrap config;

  native_methods::FocusSTMPtr focus_ptr() const { return AUTDSTMFocusFromSamplingConfig(config); }
  native_methods::GainSTMPtr gain_ptr() const { return AUTDSTMGainFromSamplingConfig(config); }
};

}  // namespace autd3::driver