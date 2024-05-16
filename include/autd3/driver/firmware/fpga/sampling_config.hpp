#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  [[nodiscard]] static native_methods::SamplingConfigWrap Freq(const Freq<uint32_t> f) { return native_methods::AUTDSamplingConfigFromFreq(f.hz()); }

  [[nodiscard]] static native_methods::SamplingConfigWrap FreqNearest(const Freq<double> f) {
    return native_methods::AUTDSamplingConfigFromFreqNearest(f.hz());
  }

  [[nodiscard]] static native_methods::SamplingConfigWrap DivisionRaw(const uint32_t div) {
    return native_methods::AUTDSamplingConfigFromDivisionRaw(div);
  }

  [[nodiscard]] static native_methods::SamplingConfigWrap Division(const uint32_t div) { return native_methods::AUTDSamplingConfigFromDivision(div); }
};

}  // namespace autd3::driver
