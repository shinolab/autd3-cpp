#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class EmissionConstraint final {
 public:
  AUTD3_API static inline const native_methods::EmissionConstraintWrap DontCare = native_methods::AUTDGainHoloConstraintDotCare();
  AUTD3_API static inline const native_methods::EmissionConstraintWrap Normalize = native_methods::AUTDGainHoloConstraintNormalize();
  AUTD3_API static native_methods::EmissionConstraintWrap Uniform(const driver::EmitIntensity value) noexcept {
    return native_methods::AUTDGainHoloConstraintUniform(value.value());
  }
  AUTD3_API static native_methods::EmissionConstraintWrap Clamp(const driver::EmitIntensity min_v, const driver::EmitIntensity max_v) noexcept {
    return native_methods::AUTDGainHoloConstraintClamp(min_v.value(), max_v.value());
  }
  AUTD3_API static native_methods::EmissionConstraintWrap Multiply(const double v) noexcept {
    return native_methods::AUTDGainHoloConstraintMultiply(v);
  }
};

}  // namespace autd3::gain::holo
