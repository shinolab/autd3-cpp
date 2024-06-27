#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct STMSamplingConfig final {
  AUTD3_API [[nodiscard]] static STMSamplingConfig Freq(const driver::Freq<float> f) {
    return STMSamplingConfig{native_methods::AUTDSTMSamplingConfigFromFreq(f.hz())};
  }

  AUTD3_API [[nodiscard]] static STMSamplingConfig FreqNearest(const driver::Freq<float> f) {
    return STMSamplingConfig{native_methods::AUTDSTMSamplingConfigFromFreqNearest(f.hz())};
  }

  AUTD3_API [[nodiscard]] static STMSamplingConfig SamplingConfig(const driver::SamplingConfig c) {
    return STMSamplingConfig{native_methods::AUTDSTMSamplingConfigFromSamplingConfig(c)};
  }

  operator native_methods::STMSamplingConfigWrap() const { return _inner; }

 private:
  explicit STMSamplingConfig(native_methods::STMSamplingConfigWrap inner) : _inner(std::move(inner)) {}

  native_methods::STMSamplingConfigWrap _inner;
};

}  // namespace autd3::driver
