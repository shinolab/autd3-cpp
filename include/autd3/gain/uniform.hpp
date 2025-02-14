#pragma once

#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

struct Uniform final : driver::Gain, driver::IntoDatagramTuple<Uniform> {
  explicit Uniform(const driver::EmitIntensity intensity, const driver::Phase phase) : intensity(intensity), phase(phase) {}
  Uniform() = delete;
  Uniform(const Uniform& obj) = default;             // LCOV_EXCL_LINE
  Uniform& operator=(const Uniform& obj) = default;  // LCOV_EXCL_LINE
  Uniform(Uniform&& obj) = default;                  // LCOV_EXCL_LINE
  Uniform& operator=(Uniform&& obj) = default;       // LCOV_EXCL_LINE
  ~Uniform() override = default;                     // LCOV_EXCL_LINE

  driver::EmitIntensity intensity;
  driver::Phase phase;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainUniform(intensity, phase);
  }
};
}  // namespace autd3::gain
