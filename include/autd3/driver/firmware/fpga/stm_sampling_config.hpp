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

  template <typename Rep, typename P>
  AUTD3_API [[nodiscard]] static STMSamplingConfig Period(const std::chrono::duration<Rep, P> period) {
    return STMSamplingConfig{native_methods::AUTDSTMSamplingConfigFromPeriod(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count())};
  }

  template <typename Rep, typename P>
  AUTD3_API [[nodiscard]] static STMSamplingConfig PeriodNearest(const std::chrono::duration<Rep, P> period) {
    return STMSamplingConfig{
        native_methods::AUTDSTMSamplingConfigFromPeriodNearest(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count())};
  }

  AUTD3_API [[nodiscard]] static STMSamplingConfig SamplingConfig(const driver::SamplingConfig c) {
    return STMSamplingConfig{native_methods::AUTDSTMSamplingConfigFromSamplingConfig(c)};
  }

  operator native_methods::STMSamplingConfigWrap() const { return _inner; }

  driver::Freq<float> freq(const size_t n) const { return validate(native_methods::AUTDSTMFreq(_inner, static_cast<uint32_t>(n))) * driver::Hz; }
  std::chrono::nanoseconds period(const size_t n) const {
    return std::chrono::nanoseconds(validate(native_methods::AUTDSTMPeriod(_inner, static_cast<uint32_t>(n))));
  }
  driver::SamplingConfig sampling_config(const size_t n) const {
    return driver::SamplingConfig{validate(native_methods::AUTDSTMSamplingSamplingConfig(_inner, static_cast<uint32_t>(n)))};
  }

 private:
  explicit STMSamplingConfig(native_methods::STMSamplingConfigWrap inner) : _inner(std::move(inner)) {}

  native_methods::STMSamplingConfigWrap _inner;
};

}  // namespace autd3::driver
