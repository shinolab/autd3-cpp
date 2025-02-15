#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

struct EmissionConstraint final {
  AUTD3_API static EmissionConstraint Normalize() noexcept { return EmissionConstraint(native_methods::AUTDGainHoloConstraintNormalize()); }
  AUTD3_API static EmissionConstraint Uniform(const driver::EmitIntensity value) noexcept {
    return EmissionConstraint(native_methods::AUTDGainHoloConstraintUniform(value));
  }
  AUTD3_API static EmissionConstraint Clamp(const driver::EmitIntensity min_v, const driver::EmitIntensity max_v) noexcept {
    return EmissionConstraint(native_methods::AUTDGainHoloConstraintClamp(min_v, max_v));
  }
  AUTD3_API static EmissionConstraint Multiply(const float v) noexcept {
    return EmissionConstraint(native_methods::AUTDGainHoloConstraintMultiply(v));
  }

  operator native_methods::EmissionConstraintWrap() const { return _inner; }

 private:
  explicit EmissionConstraint(const native_methods::EmissionConstraintWrap inner) : _inner(inner) {}

  native_methods::EmissionConstraintWrap _inner;
};

}  // namespace autd3::gain::holo
