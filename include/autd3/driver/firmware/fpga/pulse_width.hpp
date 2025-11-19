#pragma once

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct PulseWidth final {
 private:
  AUTD3_API explicit constexpr PulseWidth(const native_methods::PulseWidth inner) noexcept { _inner = inner; }

 public:
  AUTD3_API explicit PulseWidth(const uint16_t pulse_width) noexcept { _inner = native_methods::AUTDPulseWidth(pulse_width); }

  static PulseWidth from_duty(const float duty) { return PulseWidth(native_methods::AUTDPulseWidthFromDuty(duty)); }

  AUTD3_API [[nodiscard]] constexpr uint16_t pulse_width() const { return validate(native_methods::AUTDPulseWidthPulseWidth(_inner)); }

  operator native_methods::PulseWidth() const noexcept { return _inner; }

  auto operator<=>(const PulseWidth& other) const { return pulse_width() <=> other.pulse_width(); }
  auto operator==(const PulseWidth& other) const { return pulse_width() == other.pulse_width(); }

 private:
  native_methods::PulseWidth _inner;
};

}  // namespace autd3::driver
