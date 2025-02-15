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

struct FocusOption {
  driver::EmitIntensity intensity = std::numeric_limits<driver::EmitIntensity>::max();
  driver::Phase phase_offset = driver::Phase(0);
  operator native_methods::FocusOption() const { return native_methods::FocusOption{.intensity = intensity, .phase_offset = phase_offset}; }
};

struct Focus final : driver::Gain, driver::IntoDatagramTuple<Focus> {
  AUTD3_API explicit Focus(driver::Point3 pos, const FocusOption option) : pos(std::move(pos)), option(option) {}
  Focus() = delete;
  Focus(const Focus& obj) = default;             // LCOV_EXCL_LINE
  Focus& operator=(const Focus& obj) = default;  // LCOV_EXCL_LINE
  Focus(Focus&& obj) = default;                  // LCOV_EXCL_LINE
  Focus& operator=(Focus&& obj) = default;       // LCOV_EXCL_LINE
  ~Focus() override = default;                   // LCOV_EXCL_LINE

  driver::Point3 pos;
  FocusOption option;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return AUTDGainFocus(native_methods::Point3{pos.x(), pos.y(), pos.z()}, option);
  }
};

}  // namespace autd3::gain
