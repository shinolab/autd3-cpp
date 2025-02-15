#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct Static final : driver::Modulation, driver::IntoDatagramTuple<Static> {
  AUTD3_API explicit Static(const uint8_t intensity = std::numeric_limits<uint8_t>::max()) : intensity(intensity) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override { return native_methods::AUTDModulationStatic(intensity); }

  uint8_t intensity;
};
}  // namespace autd3::modulation
