#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {
struct Drive final {
  AUTD3_API static const Drive Null;

  Phase phase;
  Intensity intensity;

  auto operator<=>(const Drive&) const = default;  // LCOV_EXCL_LINE
};

inline const Drive Drive::Null = Drive{Phase::ZERO, std::numeric_limits<Intensity>::min()};

}  // namespace autd3::driver
