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
  explicit Uniform(const driver::EmitIntensity intensity) : _intensity(intensity), _phase(driver::Phase(0)) {}
  Uniform() = delete;
  Uniform(const Uniform& obj) = default;
  Uniform& operator=(const Uniform& obj) = default;
  Uniform(Uniform&& obj) = default;
  Uniform& operator=(Uniform&& obj) = default;
  ~Uniform() override = default;

  AUTD3_DEF_PROP(driver::EmitIntensity, intensity)
  AUTD3_DEF_PARAM(Uniform, driver::Phase, phase)

  [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainUniform(_intensity.value(), _phase.value());
  }
};
}  // namespace autd3::gain
