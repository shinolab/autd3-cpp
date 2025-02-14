#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

struct BesselOption {
  driver::EmitIntensity intensity = std::numeric_limits<driver::EmitIntensity>::max();
  driver::Phase phase_offset = driver::Phase(0);

  operator native_methods::BesselOption() const { return native_methods::BesselOption{.intensity = intensity, .phase_offset = phase_offset}; }
};

struct Bessel final : driver::Gain, driver::IntoDatagramTuple<Bessel> {
  AUTD3_API explicit Bessel(driver::Point3 pos, driver::Vector3 dir, const driver::Angle theta, const BesselOption option)
      : pos(std::move(pos)), dir(std::move(dir)), theta(theta), option(option) {}
  Bessel() = delete;
  Bessel(const Bessel& obj) = default;             // LCOV_EXCL_LINE
  Bessel& operator=(const Bessel& obj) = default;  // LCOV_EXCL_LINE
  Bessel(Bessel&& obj) = default;                  // LCOV_EXCL_LINE
  Bessel& operator=(Bessel&& obj) = default;       // LCOV_EXCL_LINE
  ~Bessel() override = default;                    // LCOV_EXCL_LINE

  driver::Point3 pos;
  driver::Vector3 dir;
  driver::Angle theta;
  BesselOption option;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return AUTDGainBessel(native_methods::Point3{pos.x(), pos.y(), pos.z()}, native_methods::Vector3{dir.x(), dir.y(), dir.z()}, theta, option);
  }
};

}  // namespace autd3::gain
