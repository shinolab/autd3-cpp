#pragma once

#include <chrono>

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  static SamplingConfig freq_4k() noexcept { return SamplingConfig(10); }
  static SamplingConfig freq_40k() noexcept { return SamplingConfig(1); }

  SamplingConfig into_nearest() noexcept { return SamplingConfig(native_methods::AUTDSamplingConfigIntoNearest(_inner)); }

  operator native_methods::SamplingConfigWrap() const { return _inner; }

  [[nodiscard]] uint16_t division() const { return validate(AUTDSamplingConfigDivision(_inner)); }
  [[nodiscard]] Freq<float> freq() const { return validate(AUTDSamplingConfigFreq(_inner)) * Hz; }
  [[nodiscard]] std::chrono::nanoseconds period() const { return from_duration(validate(AUTDSamplingConfigPeriod(_inner))); }

  SamplingConfig(const Freq<float> f) : _inner(native_methods::AUTDSamplingConfigFromFreq(f.hz())) {}
  template <typename Rep, typename P>
  SamplingConfig(const std::chrono::duration<Rep, P> period)
      : _inner(native_methods::AUTDSamplingConfigFromPeriod(native_methods::to_duration(period))) {}
  explicit SamplingConfig(const uint16_t div) : _inner(validate(native_methods::AUTDSamplingConfigFromDivision(div))) {}
  explicit SamplingConfig(const native_methods::SamplingConfigWrap inner) : _inner(inner) {}

  bool operator==(const SamplingConfig& other) const { return AUTDSamplingConfigEq(_inner, other._inner); }

 private:
  native_methods::SamplingConfigWrap _inner;
};

template <class C>
concept sampling_config = std::is_convertible_v<C, SamplingConfig>;

}  // namespace autd3::driver
