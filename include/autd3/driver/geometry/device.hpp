#pragma once

#include <vector>

#include "autd3/def.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Device {
  class DeviceView : public std::ranges::view_interface<DeviceView> {
   public:
    DeviceView() = default;
    AUTD3_API explicit DeviceView(const std::vector<Transducer>& vec) : _begin(vec.cbegin()), _end(vec.cend()) {}

    AUTD3_API [[nodiscard]] auto begin() const { return _begin; }
    AUTD3_API [[nodiscard]] auto end() const { return _end; }

   private:
    std::vector<Transducer>::const_iterator _begin{}, _end{};
  };

 public:
  AUTD3_API explicit Device(const size_t idx, const native_methods::DevicePtr ptr) : _idx(idx), _ptr(ptr) {
    const auto size = static_cast<size_t>(AUTDDeviceNumTransducers(_ptr));
    _transducers.clear();
    _transducers.reserve(size);
    for (uint32_t i = 0; i < size; i++) _transducers.emplace_back(i, _ptr);
  }

  ~Device() = default;
  Device(const Device& v) noexcept = default;
  Device& operator=(const Device& obj) = default;
  Device(Device&& obj) = default;
  Device& operator=(Device&& obj) = default;

  AUTD3_API [[nodiscard]] size_t idx() const { return _idx; }

  AUTD3_API [[nodiscard]] size_t num_transducers() const { return _transducers.size(); }

  AUTD3_API [[nodiscard]] Vector3 center() const {
    Vector3 v;
    AUTDDeviceCenter(_ptr, v.data());
    return v;
  }

  AUTD3_API [[nodiscard]] double sound_speed() const { return AUTDDeviceGetSoundSpeed(_ptr); }

  AUTD3_API void set_sound_speed(const double value) const { AUTDDeviceSetSoundSpeed(_ptr, value); }

  AUTD3_API void set_sound_speed_from_temp(const double temp, const double k = 1.4, const double r = 8.31446261815324,
                                           const double m = 28.9647e-3) const {
    AUTDDeviceSetSoundSpeedFromTemp(_ptr, temp, k, r, m);
  }

  AUTD3_API [[nodiscard]] double attenuation() const { return AUTDDeviceGetAttenuation(_ptr); }

  AUTD3_API void set_attenuation(const double value) const { AUTDDeviceSetAttenuation(_ptr, value); }

  AUTD3_API [[nodiscard]] bool enable() const { return AUTDDeviceEnableGet(_ptr); }

  AUTD3_API void set_enable(const bool value) const { AUTDDeviceEnableSet(_ptr, value); }

  AUTD3_API void translate(Vector3 t) const { AUTDDeviceTranslate(_ptr, t.x(), t.y(), t.z()); }

  AUTD3_API void rotate(Quaternion r) const { AUTDDeviceRotate(_ptr, r.w(), r.x(), r.y(), r.z()); }

  AUTD3_API void affine(Vector3 t, Quaternion r) const { AUTDDeviceAffine(_ptr, t.x(), t.y(), t.z(), r.w(), r.x(), r.y(), r.z()); }

  AUTD3_API [[nodiscard]] double wavelength() const { return AUTDDeviceWavelength(_ptr); }
  AUTD3_API [[nodiscard]] double wavenumber() const { return AUTDDeviceWavenumber(_ptr); }

  AUTD3_API [[nodiscard]] DeviceView transducers() const noexcept { return DeviceView(_transducers); }

  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator cbegin() const noexcept { return _transducers.cbegin(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator cend() const noexcept { return _transducers.cend(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator begin() const noexcept { return _transducers.begin(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator end() const noexcept { return _transducers.end(); }

  AUTD3_API [[nodiscard]] const Transducer& operator[](const size_t i) const { return _transducers[i]; }

  AUTD3_API [[nodiscard]] native_methods::DevicePtr ptr() const noexcept { return _ptr; }

 private:
  size_t _idx;
  native_methods::DevicePtr _ptr;
  std::vector<Transducer> _transducers{};
};

}  // namespace autd3::driver::geometry
