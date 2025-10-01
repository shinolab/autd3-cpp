#pragma once

#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>

#include "autd3/def.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::controller {
class Sender;
}

namespace autd3::driver::geometry {

template <class F>
concept reconfigure_f = requires(F f, const Device& d) {
  { f(d) } -> std::same_as<AUTD3>;
};

class Geometry : public std::ranges::view_interface<Geometry> {
 public:
  friend class autd3::controller::Sender;

  AUTD3_API explicit Geometry(const native_methods::GeometryPtr ptr) : _geometry_ptr(ptr) {
    const auto size = AUTDGeometryNumDevices(_geometry_ptr);
    _devices.clear();
    _devices.reserve(size);
    for (uint16_t i = 0; i < size; i++) _devices.emplace_back(i, _geometry_ptr);
  }

  virtual ~Geometry() = default;                       // LCOV_EXCL_LINE
  Geometry(const Geometry& v) noexcept = default;      // LCOV_EXCL_LINE
  Geometry& operator=(const Geometry& obj) = default;  // LCOV_EXCL_LINE
  Geometry(Geometry&& obj) = default;                  // LCOV_EXCL_LINE
  Geometry& operator=(Geometry&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] size_t num_devices() const { return AUTDGeometryNumDevices(_geometry_ptr); }

  AUTD3_API [[nodiscard]] size_t num_transducers() const { return AUTDGeometryNumTransducers(_geometry_ptr); }

  AUTD3_API [[nodiscard]] Point3 center() const { return AUTDGeometryCenter(_geometry_ptr); }

  template <reconfigure_f F>
  AUTD3_API void reconfigure(const F& f) {
    std::vector<AUTD3> devices;
    devices.reserve(_devices.size());
    std::ranges::transform(_devices, std::back_inserter(devices), f);

    std::vector<native_methods::Point3> pos;
    pos.reserve(devices.size());
    std::ranges::transform(devices, std::back_inserter(pos), [&](const auto& d) { return native_methods::Point3{d.pos.x(), d.pos.y(), d.pos.z()}; });

    std::vector<native_methods::Quaternion> rot;
    rot.reserve(devices.size());
    std::ranges::transform(devices, std::back_inserter(rot),
                           [&](const auto& d) { return native_methods::Quaternion{d.rot.x(), d.rot.y(), d.rot.z(), d.rot.w()}; });

    native_methods::AUTDGeometryReconfigure(_geometry_ptr, pos.data(), rot.data());
    for (size_t i = 0; i < _devices.size(); i++) _devices[i] = Device(static_cast<uint16_t>(i), _geometry_ptr);
  }

  [[nodiscard]] std::vector<Device>::iterator begin() noexcept { return _devices.begin(); }
  [[nodiscard]] std::vector<Device>::iterator end() noexcept { return _devices.end(); }
  [[nodiscard]] std::vector<Device>::const_iterator begin() const noexcept { return _devices.cbegin(); }
  [[nodiscard]] std::vector<Device>::const_iterator end() const noexcept { return _devices.cend(); }
  [[nodiscard]] std::vector<Device>::const_iterator cbegin() const noexcept { return _devices.cbegin(); }
  [[nodiscard]] std::vector<Device>::const_iterator cend() const noexcept { return _devices.cend(); }
  [[nodiscard]] const Device& operator[](const size_t i) const { return _devices[i]; }
  [[nodiscard]] Device& operator[](const size_t i) { return _devices[i]; }
  [[nodiscard]] native_methods::GeometryPtr ptr() const noexcept { return _geometry_ptr; }

 protected:
  explicit Geometry(const native_methods::GeometryPtr geometry_ptr, std::vector<Device> devices)
      : _geometry_ptr(geometry_ptr), _devices(std::move(devices)) {}

  native_methods::GeometryPtr _geometry_ptr;
  std::vector<Device> _devices{};
};

}  // namespace autd3::driver::geometry
