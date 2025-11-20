#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {

struct ControlPoint {
  Point3 point;
  Phase phase_offset = Phase::ZERO;
};

template <uint8_t N>
struct ControlPoints {
  std::array<ControlPoint, N> points = {};
  Intensity intensity = std::numeric_limits<Intensity>::max();
};

}  // namespace autd3::driver
