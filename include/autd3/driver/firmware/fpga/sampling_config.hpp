#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  AUTD3_API [[nodiscard]] static SamplingConfig Freq(const driver::Freq<uint32_t> f) { return SamplingConfig{native_methods::AUTDSamplingConfigFromFreq(f.hz())}; }

  AUTD3_API [[nodiscard]] static SamplingConfig FreqNearest(const driver::Freq<float> f) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromFreqNearest(f.hz())};
  }

  AUTD3_API [[nodiscard]] static SamplingConfig DivisionRaw(const uint32_t div) { return SamplingConfig{native_methods::AUTDSamplingConfigFromDivisionRaw(div)}; }

  AUTD3_API [[nodiscard]] static SamplingConfig Division(const uint32_t div) { return SamplingConfig{native_methods::AUTDSamplingConfigFromDivision(div)}; }

	operator native_methods::SamplingConfigWrap() const { return _inner; }

 private:
 explicit  SamplingConfig(native_methods::SamplingConfigWrap inner) : _inner(std::move(inner)) {  }

  native_methods::SamplingConfigWrap _inner;
};

}  // namespace autd3::driver
