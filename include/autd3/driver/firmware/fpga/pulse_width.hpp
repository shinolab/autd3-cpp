#pragma once

#include <stdexcept>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct PulseWidth final {
  template <std::integral T>
  AUTD3_API explicit constexpr PulseWidth(const T pulse_width) {
    if (pulse_width < 0 || 512 <= pulse_width) throw std::out_of_range("PulseWidth must be in range [0, 255]");
    _pulse_width = static_cast<uint16_t>(pulse_width);
  }

  static PulseWidth from_duty(const float duty) {
    if (duty < 0.0f || 1.0f < duty) throw std::out_of_range("Duty must be in range [0, 1]");
    return PulseWidth(static_cast<uint16_t>(duty * 512.0f));
  }

  AUTD3_API [[nodiscard]] constexpr uint16_t pulse_width() const noexcept { return _pulse_width; }

  auto operator<=>(const PulseWidth&) const = default;  // LCOV_EXCL_LINE

 private:
  uint16_t _pulse_width;
};

}  // namespace autd3::driver
