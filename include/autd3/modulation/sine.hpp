#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/modulation/sampling_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SineOption {
  uint8_t intensity = 0xFF;
  uint8_t offset = 0x80;
  driver::Angle phase = 0.0 * driver::rad;
  bool clamp = false;
  driver::SamplingConfig sampling_config = driver::SamplingConfig::freq_4k();

  operator native_methods::SineOption() const {
    return native_methods::SineOption{
        .intensity = intensity, .offset = offset, .phase = phase, .clamp = clamp, .sampling_config_div = sampling_config.division()};
  }
};

template <typename>
struct Sine {};

template <>
struct Sine<driver::Freq<uint32_t>> final : driver::Modulation, driver::IntoDatagramTuple<Sine<driver::Freq<uint32_t>>> {
  AUTD3_API explicit Sine(const driver::Freq<uint32_t> freq, const SineOption option) : freq(freq), option(option) {}

  driver::Freq<uint32_t> freq;
  SineOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSineExact(freq.hz(), option);
  }
};

template <>
struct Sine<Nearest> final : driver::Modulation, driver::IntoDatagramTuple<Sine<driver::Freq<float>>> {
  friend struct Sine<driver::Freq<float>>;

  driver::Freq<float> freq;
  SineOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSineNearest(freq.hz(), option);
  }

 private:
  AUTD3_API explicit Sine(const driver::Freq<float> freq, const SineOption option) : freq(freq), option(option) {}
};

template <>
struct Sine<driver::Freq<float>> final : driver::Modulation, driver::IntoDatagramTuple<Sine<driver::Freq<float>>> {
  AUTD3_API explicit Sine(const driver::Freq<float> freq, const SineOption option) : freq(freq), option(option) {}

  driver::Freq<float> freq;
  SineOption option;

  [[nodiscard]] Sine<Nearest> into_nearest() const { return Sine<Nearest>(freq, option); }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSineExactFloat(freq.hz(), option);
  }
};

Sine(driver::Freq<uint32_t>&& freq, SineOption option) -> Sine<driver::Freq<uint32_t>>;
Sine(driver::Freq<float>&& freq, SineOption option) -> Sine<driver::Freq<float>>;

}  // namespace autd3::modulation
