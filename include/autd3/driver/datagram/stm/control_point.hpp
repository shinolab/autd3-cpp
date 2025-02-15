#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {

struct ControlPoint {
  Point3 point;
  Phase phase_offset = Phase::zero();
};

template <uint8_t N>
struct ControlPoints {
  std::array<ControlPoint, N> points = {};
  EmitIntensity intensity = std::numeric_limits<EmitIntensity>::max();
};

}  // namespace autd3::driver
