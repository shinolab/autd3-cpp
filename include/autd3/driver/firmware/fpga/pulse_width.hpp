#pragma once

#include <stdexcept>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct PulseWidth final {
 private:
  struct _empty {};
  AUTD3_API explicit constexpr PulseWidth(const uint16_t pulse_width, const _empty) { _pulse_width = pulse_width; }

 public:
  AUTD3_API explicit constexpr PulseWidth(const uint16_t pulse_width) { _pulse_width = validate(native_methods::AUTDPulseWidth(pulse_width)); }

  static PulseWidth from_duty(const float duty) { return PulseWidth(validate(native_methods::AUTDPulseWidthFromDuty(duty)), _empty{}); }

  AUTD3_API [[nodiscard]] constexpr uint16_t pulse_width() const noexcept { return _pulse_width; }

  auto operator<=>(const PulseWidth&) const = default;  // LCOV_EXCL_LINE

 private:
  uint16_t _pulse_width;
};

}  // namespace autd3::driver
