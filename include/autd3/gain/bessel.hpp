#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Bessel final : public driver::Gain<Bessel> {
 public:
  AUTD3_API explicit Bessel(driver::Vector3 p, driver::Vector3 d, const double theta)
      : _pos(std::move(p)),
        _dir(std::move(d)),
        _theta(theta),
        _intensity(std::numeric_limits<driver::EmitIntensity>::max()),
        _phase_offset(driver::Phase(0)) {}
  Bessel() = delete;
  Bessel(const Bessel& obj) = default;
  Bessel& operator=(const Bessel& obj) = default;
  Bessel(Bessel&& obj) = default;
  Bessel& operator=(Bessel&& obj) = default;
  ~Bessel() override = default;

  AUTD3_DEF_PROP(driver::Vector3, pos)
  AUTD3_DEF_PROP(driver::Vector3, dir)
  AUTD3_DEF_PROP(double, theta)
  AUTD3_DEF_PARAM(Bessel, driver::EmitIntensity, intensity)
  AUTD3_DEF_PARAM(Bessel, driver::Phase, phase_offset)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainBessel(_pos.x(), _pos.y(), _pos.z(), _dir.x(), _dir.y(), _dir.z(), _theta, _intensity.value(),
                                          _phase_offset.value());
  }
};

}  // namespace autd3::gain
