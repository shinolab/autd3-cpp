#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct STMSamplingConfig final {
  STMSamplingConfig() : _inner(SamplingConfig(1)), _n(0) {}

  operator native_methods::SamplingConfig() const { return _inner; }

  Freq<float> freq() const { return AUTDSTMFreq(_inner, _n) * driver::Hz; }
  [[nodiscard]] std::chrono::nanoseconds period() const { return std::chrono::nanoseconds(AUTDSTMPeriod(_inner, _n)); }
  driver::SamplingConfig sampling_config() const { return driver::SamplingConfig{_inner}; }

  static STMSamplingConfig nearest(const Freq<float> freq, const uint16_t n) {
    return STMSamplingConfig{validate(native_methods::AUTDSTMConfigFromFreqNearest(freq.hz(), n)), n};
  }

  template <typename Rep, typename P>
  static STMSamplingConfig nearest(const std::chrono::duration<Rep, P> period, const uint16_t n) {
    return STMSamplingConfig{
        validate(native_methods::AUTDSTMConfigFromPeriodNearest(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(), n)), n};
  }

  explicit STMSamplingConfig(const driver::SamplingConfig config, const uint16_t n) : _inner(config), _n(n) {}
  explicit STMSamplingConfig(const driver::Freq<float> freq, const uint16_t n)
      : _inner(validate(native_methods::AUTDSTMConfigFromFreq(freq.hz(), n))), _n(n) {}
  template <typename Rep, typename P>
  explicit STMSamplingConfig(const std::chrono::duration<Rep, P> period, const uint16_t n)
      : _inner(validate(native_methods::AUTDSTMConfigFromPeriod(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(), n))), _n(n) {}

  explicit STMSamplingConfig(const STMSamplingConfig config, const uint16_t n) : _inner(config._inner), _n(n) {}

 private:
  explicit STMSamplingConfig(const native_methods::SamplingConfig config, const uint16_t n) : _inner(config), _n(n) {}

  native_methods::SamplingConfig _inner;
  uint16_t _n;
};

template <class C>
concept stm_sampling_config = std::is_constructible_v<STMSamplingConfig, C, uint16_t>;

}  // namespace autd3::driver
