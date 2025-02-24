#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/modulation/sampling_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SquareOption {
  uint8_t low = 0x00;
  uint8_t high = 0xFF;
  float duty = 0.5;
  driver::SamplingConfig sampling_config = driver::SamplingConfig::freq_4k();

  operator native_methods::SquareOption() const {
    return native_methods::SquareOption{.low = low, .high = high, .duty = duty, .sampling_config_div = sampling_config.division()};
  }
};

template <typename>
struct Square {};

template <>
struct Square<driver::Freq<uint32_t>> final : driver::Modulation, driver::IntoDatagramTuple<Square<driver::Freq<uint32_t>>> {
  AUTD3_API explicit Square(const driver::Freq<uint32_t> freq, const SquareOption option) : freq(freq), option(option) {}

  driver::Freq<uint32_t> freq;
  SquareOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSquareExact(freq.hz(), option);
  }
};

template <>
struct Square<Nearest> final : driver::Modulation, driver::IntoDatagramTuple<Square<driver::Freq<float>>> {
  friend struct Square<driver::Freq<float>>;

  driver::Freq<float> freq;
  SquareOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSquareNearest(freq.hz(), option);
  }

 private:
  AUTD3_API explicit Square(const driver::Freq<float> freq, const SquareOption option) : freq(freq), option(option) {}
};

template <>
struct Square<driver::Freq<float>> final : driver::Modulation, driver::IntoDatagramTuple<Square<driver::Freq<float>>> {
  AUTD3_API explicit Square(const driver::Freq<float> freq, const SquareOption option) : freq(freq), option(option) {}

  driver::Freq<float> freq;
  SquareOption option;

  [[nodiscard]] Square<Nearest> into_nearest() const { return Square<Nearest>(freq, option); }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationSquareExactFloat(freq.hz(), option);
  }
};

Square(driver::Freq<uint32_t>&& freq, SquareOption option) -> Square<driver::Freq<uint32_t>>;
Square(driver::Freq<float>&& freq, SquareOption option) -> Square<driver::Freq<float>>;

}  // namespace autd3::modulation
