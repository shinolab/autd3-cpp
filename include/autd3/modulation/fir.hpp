#pragma once

#include <ranges>

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
struct Fir final : driver::Modulation, driver::IntoDatagramTuple<Fir<M>> {
  AUTD3_API explicit Fir(M target, std::vector<float> coef) : target(std::move(target)), coef(std::move(coef)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationWithFir(target.modulation_ptr(), coef.data(), static_cast<uint32_t>(coef.size()));
  }

  M target;
  std::vector<float> coef;
};

}  // namespace autd3::modulation
