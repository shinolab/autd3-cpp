#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

struct PlaneOption {
  driver::EmitIntensity intensity = std::numeric_limits<driver::EmitIntensity>::max();
  driver::Phase phase_offset = driver::Phase(0);

  operator native_methods::PlaneOption() const { return native_methods::PlaneOption{.intensity = intensity, .phase_offset = phase_offset}; }
};

struct Plane final : driver::Gain, driver::IntoDatagramTuple<Plane> {
  AUTD3_API explicit Plane(driver::Vector3 dir, const PlaneOption option) : dir(std::move(dir)), option(option) {}
  Plane() = delete;
  Plane(const Plane& obj) = default;             // LCOV_EXCL_LINE
  Plane& operator=(const Plane& obj) = default;  // LCOV_EXCL_LINE
  Plane(Plane&& obj) = default;                  // LCOV_EXCL_LINE
  Plane& operator=(Plane&& obj) = default;       // LCOV_EXCL_LINE
  ~Plane() override = default;                   // LCOV_EXCL_LINE

  driver::Vector3 dir;
  PlaneOption option;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return AUTDGainPlane(native_methods::Vector3{dir.x(), dir.y(), dir.z()}, option);
  }
};

}  // namespace autd3::gain
