#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct Custom final : driver::Modulation, driver::IntoDatagramTuple<Custom> {
  template <driver::sampling_config T>
  explicit Custom(std::vector<uint8_t> buffer, const T sampling_config) : buffer(std::move(buffer)), sampling_config(sampling_config) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    const auto size = buffer.size();
    return native_methods::AUTDModulationCustom(buffer.data(), static_cast<uint16_t>(size), sampling_config);
  }

  std::vector<uint8_t> buffer;
  driver::SamplingConfig sampling_config;
};

}  // namespace autd3::modulation
