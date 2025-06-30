#pragma once

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3 {
class Environment {
 public:
  Environment() : _ptr(native_methods::EnvironmentPtr{._0 = nullptr}) {}
  AUTD3_API explicit Environment(const native_methods::EnvironmentPtr ptr) : _ptr(ptr) {}

  ~Environment() = default;                                  // LCOV_EXCL_LINE
  Environment(const Environment& v) noexcept = default;      // LCOV_EXCL_LINE
  Environment& operator=(const Environment& obj) = default;  // LCOV_EXCL_LINE
  Environment(Environment&& obj) = default;                  // LCOV_EXCL_LINE
  Environment& operator=(Environment&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] float sound_speed() const { return AUTDEnvironmentGetSoundSpeed(_ptr); }

  AUTD3_API void set_sound_speed(const float value) const { AUTDEnvironmentSetSoundSpeed(_ptr, value); }

  AUTD3_API void set_sound_speed_from_temp(const float temp, const float k = 1.4f, const float r = 8.31446261815324f,
                                           const float m = 28.9647e-3f) const {
    AUTDEnvironmentSetSoundSpeedFromTemp(_ptr, temp, k, r, m);
  }

  AUTD3_API [[nodiscard]] float wavelength() const { return AUTDEnvironmentWavelength(_ptr); }
  AUTD3_API [[nodiscard]] float wavenumber() const { return AUTDEnvironmentWavenumber(_ptr); }

 private:
  native_methods::EnvironmentPtr _ptr;
};
}  // namespace autd3