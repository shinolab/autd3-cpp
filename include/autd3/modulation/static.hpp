#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Static final : public driver::ModulationBase<Static>,
                     public driver::IntoModulationCache<Static>,
                     public driver::IntoRadiationPressure<Static>,
                     public driver::IntoModulationTransform<Static> {
 public:
  Static() : ModulationBase(), _intensity(std::numeric_limits<uint8_t>::max()) {}
  explicit Static(const uint8_t intensity) : _intensity(intensity) {}

  [[nodiscard]] static Static with_intensity(const uint8_t intensity) { return Static(intensity); }

  [[nodiscard]] uint8_t intensity() const { return _intensity; }

  [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationStatic(_intensity, static_cast<native_methods::LoopBehavior>(_loop_behavior));
  }

 private:
  uint8_t _intensity;
};
}  // namespace autd3::modulation
