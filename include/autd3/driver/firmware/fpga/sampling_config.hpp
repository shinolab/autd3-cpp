#pragma once

#include <chrono>

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  AUTD3_API [[nodiscard]] static SamplingConfig Freq(const driver::Freq<uint32_t> f) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromFreq(f.hz())};
  }

  AUTD3_API [[nodiscard]] static SamplingConfig FreqNearest(const driver::Freq<float> f) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromFreqNearest(f.hz())};
  }

  AUTD3_API [[nodiscard]] static SamplingConfig DivisionRaw(const uint32_t div) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromDivisionRaw(div)};
  }

  AUTD3_API [[nodiscard]] static SamplingConfig Division(const uint32_t div) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromDivision(div)};
  }

  template <typename Rep, typename P>
  AUTD3_API [[nodiscard]] static SamplingConfig Period(const std::chrono::duration<Rep, P> period) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromPeriod(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count())};
  }

  template <typename Rep, typename P>
  AUTD3_API [[nodiscard]] static SamplingConfig PeriodNearest(const std::chrono::duration<Rep, P> period) {
    return SamplingConfig{native_methods::AUTDSamplingConfigFromPeriodNearest(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count())};
  }

  operator native_methods::SamplingConfigWrap() const { return _inner; }

  uint32_t division() const { return validate(native_methods::AUTDSamplingConfigDivision(_inner)); }
  driver::Freq<float> freq() const { return validate(native_methods::AUTDSamplingConfigFreq(_inner)) * driver::Hz; }
  std::chrono::nanoseconds period() const { return std::chrono::nanoseconds(validate(native_methods::AUTDSamplingConfigPeriod(_inner))); }

  explicit SamplingConfig(native_methods::SamplingConfigWrap inner) : _inner(std::move(inner)) {}

 private:
  native_methods::SamplingConfigWrap _inner;
};

}  // namespace autd3::driver
