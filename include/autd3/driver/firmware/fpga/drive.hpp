#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {
struct Drive final {
  Phase phase;
  EmitIntensity intensity;

  explicit constexpr Drive(const Phase phase, const EmitIntensity intensity) : phase(phase), intensity(intensity) {}
  explicit constexpr Drive(const Phase phase, const uint8_t intensity) : phase(phase), intensity(intensity) {}
  static constexpr Drive null() noexcept { return Drive{Phase(0x00), EmitIntensity(0x00)}; }

  auto operator<=>(const Drive&) const = default;
};

}  // namespace autd3::driver
