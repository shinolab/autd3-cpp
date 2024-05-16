#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Focus final : public driver::Gain<Focus> {
 public:
  explicit Focus(driver::Vector3 p)
      : _pos(std::move(p)), _intensity(std::numeric_limits<driver::EmitIntensity>::max()), _phase_offset(driver::Phase(0)) {}
  Focus() = delete;
  Focus(const Focus& obj) = default;
  Focus& operator=(const Focus& obj) = default;
  Focus(Focus&& obj) = default;
  Focus& operator=(Focus&& obj) = default;
  ~Focus() override = default;

  AUTD3_DEF_PROP(driver::Vector3, pos)
  AUTD3_DEF_PARAM(Focus, driver::EmitIntensity, intensity)
  AUTD3_DEF_PARAM(Focus, driver::Phase, phase_offset)

  [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainFocus(_pos.x(), _pos.y(), _pos.z(), _intensity.value(), _phase_offset.value());
  }
};

}  // namespace autd3::gain
