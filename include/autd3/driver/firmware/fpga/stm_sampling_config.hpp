#pragma once

#include <chrono>

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct STMSamplingConfig final {
  static SamplingConfig from_freq(const Freq<float> freq, const size_t n) {
    return SamplingConfig{validate(native_methods::AUTDSTMConfigFromFreq(freq.hz(), static_cast<uint16_t>(n)))};
  }

  static SamplingConfig from_freq_nearest(const Freq<float> freq, const size_t n) {
    return SamplingConfig{native_methods::AUTDSTMConfigFromFreqNearest(freq.hz(), static_cast<uint16_t>(n))};
  }

  static SamplingConfig from_period(const std::chrono::nanoseconds period, const size_t n) {
    return SamplingConfig{validate(AUTDSTMConfigFromPeriod(native_methods::to_duration(period), static_cast<uint16_t>(n)))};
  }

  static SamplingConfig from_period_nearest(const std::chrono::nanoseconds period, const size_t n) {
    return SamplingConfig{AUTDSTMConfigFromPeriodNearest(native_methods::to_duration(period), static_cast<uint16_t>(n))};
  }
};

}  // namespace autd3::driver
