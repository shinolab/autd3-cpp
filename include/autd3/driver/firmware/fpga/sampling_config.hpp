#pragma once

#include <chrono>

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  static SamplingConfig nearest(const driver::Freq<float> f) noexcept {
    return SamplingConfig(native_methods::AUTDSamplingConfigFromFreqNearest(f.hz()));
  }
  template <typename Rep, typename P>
  static SamplingConfig nearest(const std::chrono::duration<Rep, P> period) noexcept {
    return SamplingConfig(native_methods::AUTDSamplingConfigFromPeriodNearest(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count()));
  }

  operator native_methods::SamplingConfig() const { return _inner; }

  uint32_t division() const { return native_methods::AUTDSamplingConfigDivision(_inner); }
  driver::Freq<float> freq() const { return native_methods::AUTDSamplingConfigFreq(_inner) * driver::Hz; }
  std::chrono::nanoseconds period() const { return std::chrono::nanoseconds(native_methods::AUTDSamplingConfigPeriod(_inner)); }

  SamplingConfig(const driver::Freq<uint32_t> f) : _inner(validate(native_methods::AUTDSamplingConfigFromFreq(f.hz()))) {}
  SamplingConfig(const driver::Freq<float> f) : _inner(validate(native_methods::AUTDSamplingConfigFromFreqF(f.hz()))) {}
  template <typename Rep, typename P>
  SamplingConfig(const std::chrono::duration<Rep, P> period)
      : _inner(validate(native_methods::AUTDSamplingConfigFromPeriod(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count()))) {}
  explicit SamplingConfig(const uint16_t div) : _inner(native_methods::AUTDSamplingConfigFromDivision(div)) {}
  explicit SamplingConfig(native_methods::SamplingConfig inner) : _inner(std::move(inner)) {}

  bool operator==(const SamplingConfig& other) const { return _inner._div == other._inner._div; }

 private:
  native_methods::SamplingConfig _inner;
};

template <class C>
concept sampling_config = std::is_convertible_v<C, SamplingConfig>;

}  // namespace autd3::driver
