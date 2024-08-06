#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct STMSamplingConfig final {
  STMSamplingConfig() : _inner(native_methods::SamplingConfig(0)), _n(0) {};

  operator native_methods::SamplingConfig() const { return _inner; }

  driver::Freq<float> freq() const { return native_methods::AUTDSTMFreq(_inner, _n) * driver::Hz; }
  std::chrono::nanoseconds period() const { return std::chrono::nanoseconds(native_methods::AUTDSTMPeriod(_inner, _n)); }
  driver::SamplingConfig sampling_config() const { return driver::SamplingConfig{_inner}; }

  static STMSamplingConfig nearest(const driver::Freq<float> freq, const uint32_t n) {
    return STMSamplingConfig{validate(native_methods::AUTDSTMConfigFromFreqNearest(freq.hz(), n)), n};
  }

  template <typename Rep, typename P>
  static STMSamplingConfig nearest(const std::chrono::duration<Rep, P> period, const uint32_t n) {
    return STMSamplingConfig{
        validate(native_methods::AUTDSTMConfigFromPeriodNearest(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(), n)), n};
  }

  explicit STMSamplingConfig(driver::SamplingConfig config, const uint32_t n) : _inner(std::move(config)), _n(n) {}
  explicit STMSamplingConfig(const driver::Freq<float> freq, const uint32_t n)
      : _inner(validate(native_methods::AUTDSTMConfigFromFreq(freq.hz(), n))), _n(n) {}
  template <typename Rep, typename P>
  explicit STMSamplingConfig(const std::chrono::duration<Rep, P> period, const uint32_t n)
      : _inner(validate(native_methods::AUTDSTMConfigFromPeriod(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(), n))), _n(n) {}

  explicit STMSamplingConfig(STMSamplingConfig config, const uint32_t n) : _inner(std::move(config._inner)), _n(n) {}

 private:
  explicit STMSamplingConfig(native_methods::SamplingConfig config, const uint32_t n) : _inner(std::move(config)), _n(n) {}

  native_methods::SamplingConfig _inner;
  uint32_t _n;
};

template <class C>
concept stm_sampling_config = std::is_constructible_v<STMSamplingConfig, C, uint32_t>;

}  // namespace autd3::driver
