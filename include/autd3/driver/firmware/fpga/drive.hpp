#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {
struct Drive final {
  Phase phase;
  EmitIntensity intensity;

  AUTD3_API constexpr static Drive null() noexcept { return Drive{Phase::zero(), std::numeric_limits<EmitIntensity>::min()}; }

  auto operator<=>(const Drive&) const = default;  // LCOV_EXCL_LINE
};

}  // namespace autd3::driver
