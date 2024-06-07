#pragma once

#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>

#include "autd3/def.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Geometry {
  class GeometryView : public std::ranges::view_interface<GeometryView> {
   public:
    GeometryView() = default;  // LCOV_EXCL_LINE
    AUTD3_API explicit GeometryView(const std::vector<Device>& vec) : _begin(vec.cbegin()), _end(vec.cend()) {}

    AUTD3_API [[nodiscard]] auto begin() const { return _begin; }
    AUTD3_API [[nodiscard]] auto end() const { return _end; }

   private:
    std::vector<Device>::const_iterator _begin{}, _end{};
  };

 public:
  AUTD3_API explicit Geometry(const native_methods::GeometryPtr ptr) : _ptr(ptr) {
    const auto size = AUTDGeometryNumDevices(_ptr);
    _devices.clear();
    _devices.reserve(size);
    for (uint16_t i = 0; i < size; i++) _devices.emplace_back(static_cast<size_t>(i), AUTDDevice(_ptr, i));
  }

  ~Geometry() = default;                               // LCOV_EXCL_LINE
  Geometry(const Geometry& v) noexcept = default;      // LCOV_EXCL_LINE
  Geometry& operator=(const Geometry& obj) = default;  // LCOV_EXCL_LINE
  Geometry(Geometry&& obj) = default;                  // LCOV_EXCL_LINE
  Geometry& operator=(Geometry&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] size_t num_devices() const { return _devices.size(); }

  AUTD3_API [[nodiscard]] size_t num_transducers() const {
    return std::accumulate(_devices.begin(), _devices.end(), size_t{0}, [](const size_t acc, const Device& d) { return acc + d.num_transducers(); });
  }

  AUTD3_API [[nodiscard]] Vector3 center() const {
    return std::accumulate(_devices.begin(), _devices.end(), Vector3(0, 0, 0),
                           [](const Vector3& acc, const Device& d) -> Vector3 {
                             Vector3 res = acc + d.center();
                             return res;
                           }) /
           static_cast<float>(num_devices());
  }

  AUTD3_API [[nodiscard]] auto devices() const noexcept {
    return GeometryView(_devices) | std::views::filter([](const auto& dev) { return dev.enable(); });
  }

  AUTD3_API void set_sound_speed(const float value) const {
    std::ranges::for_each(devices(), [value](const auto& dev) { dev.set_sound_speed(value); });
  }

  AUTD3_API void set_sound_speed_from_temp(const float temp, const float k = 1.4f, const float r = 8.31446261815324f,
                                           const float m = 28.9647e-3f) const {
    std::ranges::for_each(devices(), [temp, k, r, m](const auto& dev) { dev.set_sound_speed_from_temp(temp, k, r, m); });
  }

  AUTD3_API [[nodiscard]] std::vector<Device>::iterator begin() noexcept { return _devices.begin(); }
  AUTD3_API [[nodiscard]] std::vector<Device>::iterator end() noexcept { return _devices.end(); }
  AUTD3_API [[nodiscard]] std::vector<Device>::const_iterator cbegin() const noexcept { return _devices.cbegin(); }
  AUTD3_API [[nodiscard]] std::vector<Device>::const_iterator cend() const noexcept { return _devices.cend(); }
  AUTD3_API [[nodiscard]] const Device& operator[](const size_t i) const { return _devices[i]; }
  AUTD3_API [[nodiscard]] Device& operator[](const size_t i) { return _devices[i]; }
  AUTD3_API [[nodiscard]] native_methods::GeometryPtr ptr() const noexcept { return _ptr; }

 private:
  native_methods::GeometryPtr _ptr;
  std::vector<Device> _devices{};
};

}  // namespace autd3::driver::geometry
