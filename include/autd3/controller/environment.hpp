#pragma once

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3 {
class Environment {
 public:
  AUTD3_API explicit Environment(const native_methods::EnvironmentPtr ptr) : sound_speed(0.f, ptr), _ptr(ptr) {}

  ~Environment() = default;                                  // LCOV_EXCL_LINE
  Environment(const Environment& v) noexcept = default;      // LCOV_EXCL_LINE
  Environment& operator=(const Environment& obj) = default;  // LCOV_EXCL_LINE
  Environment(Environment&& obj) = default;                  // LCOV_EXCL_LINE
  Environment& operator=(Environment&& obj) = default;       // LCOV_EXCL_LINE

  struct SoundSpeedGetter {
    static float get(const float&, const native_methods::EnvironmentPtr ptr) { return AUTDEnvironmentGetSoundSpeed(ptr); }
  };
  struct SoundSpeedSetter {
    static void set(float&, const native_methods::EnvironmentPtr ptr, const float& value) { AUTDEnvironmentSetSoundSpeed(ptr, value); }
  };

  native_methods::Property<float, native_methods::EnvironmentPtr, SoundSpeedGetter, SoundSpeedSetter> sound_speed;

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