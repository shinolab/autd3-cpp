#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Static final : public driver::ModulationBase<Static>,
                     public driver::IntoModulationCache<Static>,
                     public driver::IntoRadiationPressure<Static>,
                     public driver::IntoModulationTransform<Static> {
 public:
  AUTD3_API Static() : ModulationBase(), _intensity(std::numeric_limits<driver::EmitIntensity>::max()) {}
  AUTD3_API explicit Static(const driver::EmitIntensity intensity) : _intensity(intensity) {}

  AUTD3_API [[nodiscard]] static Static with_intensity(const driver::EmitIntensity intensity) { return Static(intensity); }

  AUTD3_API [[nodiscard]] driver::EmitIntensity intensity() const { return _intensity; }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDModulationStatic(_intensity.value(), static_cast<native_methods::LoopBehavior>(_loop_behavior));
  }

 private:
  driver::EmitIntensity _intensity;
};
}  // namespace autd3::modulation
