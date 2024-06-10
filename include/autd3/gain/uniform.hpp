#pragma once

#include <algorithm>

#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Uniform final : public driver::Gain<Uniform> {
 public:
  AUTD3_API explicit Uniform(const driver::EmitIntensity intensity) : _intensity(intensity), _phase(driver::Phase(0)) {}
  Uniform() = delete;
  Uniform(const Uniform& obj) = default;             // LCOV_EXCL_LINE
  Uniform& operator=(const Uniform& obj) = default;  // LCOV_EXCL_LINE
  Uniform(Uniform&& obj) = default;                  // LCOV_EXCL_LINE
  Uniform& operator=(Uniform&& obj) = default;       // LCOV_EXCL_LINE
  ~Uniform() override = default;                     // LCOV_EXCL_LINE

  AUTD3_DEF_PROP(driver::EmitIntensity, intensity)
  AUTD3_DEF_PARAM_PHASE(Uniform, phase)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainUniform(_intensity.value(), _phase.value());
  }
};
}  // namespace autd3::gain
