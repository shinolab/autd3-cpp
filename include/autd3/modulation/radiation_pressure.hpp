#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
struct RadiationPressure final : driver::Modulation, driver::IntoDatagramTuple<RadiationPressure<M>> {
  AUTD3_API explicit RadiationPressure(M target) : target(std::move(target)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationWithRadiationPressure(target.modulation_ptr());
  }

  M target;
};

}  // namespace autd3::modulation
