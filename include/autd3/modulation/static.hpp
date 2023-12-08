// File: static.hpp
// Project: modulation
// Created Date: 13/09/2023
// Author: Shun Suzuki
// -----
// Last Modified: 28/11/2023
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2023 Shun Suzuki. All rights reserved.
//

#pragma once

#include "autd3/internal/emit_intensity.hpp"
#include "autd3/internal/native_methods.hpp"
#include "autd3/internal/utils.hpp"
#include "autd3/modulation/cache.hpp"
#include "autd3/modulation/radiation_pressure.hpp"
#include "autd3/modulation/transform.hpp"

namespace autd3::modulation {

/**
 * @brief Without modulation
 */
class Static final : public internal::Modulation, public IntoCache<Static>, public IntoRadiationPressure<Static>, public IntoTransform<Static> {
 public:
  Static() = default;

  AUTD3_DEF_PARAM_INTENSITY(Static, intensity)

  [[nodiscard]] internal::native_methods::ModulationPtr modulation_ptr() const override {
    auto ptr = internal::native_methods::AUTDModulationStatic();
    if (_intensity.has_value()) ptr = AUTDModulationStaticWithIntensity(ptr, _intensity.value().value());
    return ptr;
  }

 private:
  std::optional<internal::EmitIntensity> _intensity;
};
}  // namespace autd3::modulation
