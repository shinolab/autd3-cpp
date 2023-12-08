// File: bessel.hpp
// Project: gain
// Created Date: 13/09/2023
// Author: Shun Suzuki
// -----
// Last Modified: 02/12/2023
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2023 Shun Suzuki. All rights reserved.
//

#pragma once

#include <algorithm>
#include <optional>

#include "autd3/gain/cache.hpp"
#include "autd3/gain/transform.hpp"
#include "autd3/internal/def.hpp"
#include "autd3/internal/emit_intensity.hpp"
#include "autd3/internal/gain.hpp"
#include "autd3/internal/geometry/geometry.hpp"
#include "autd3/internal/native_methods.hpp"
#include "autd3/internal/utils.hpp"

namespace autd3::gain {

/**
 * @brief Gain to produce a Bessel beam
 */
class Bessel final : public internal::Gain, public IntoCache<Bessel>, public IntoTransform<Bessel> {
 public:
  explicit Bessel(internal::Vector3 p, internal::Vector3 d, const double theta) : _p(std::move(p)), _d(std::move(d)), _theta(theta) {}

  AUTD3_DEF_PARAM_INTENSITY(Bessel, intensity)

  [[nodiscard]] internal::native_methods::GainPtr gain_ptr(const internal::geometry::Geometry&) const override {
    auto ptr = internal::native_methods::AUTDGainBessel(_p.x(), _p.y(), _p.z(), _d.x(), _d.y(), _d.z(), _theta);
    if (_intensity.has_value()) ptr = AUTDGainBesselWithIntensity(ptr, _intensity.value().value());
    return ptr;
  }

 private:
  internal::Vector3 _p;
  internal::Vector3 _d;
  double _theta;
  std::optional<internal::EmitIntensity> _intensity;
};

}  // namespace autd3::gain
