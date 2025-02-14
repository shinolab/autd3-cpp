#pragma once

#include "autd3/driver/defined/angle.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct Phase final {
  AUTD3_API explicit constexpr Phase(const uint8_t value) : _value(value) {}
  AUTD3_API explicit Phase(const Angle value) : _value(native_methods::AUTDPhaseFromRad(value.radian())) {}

  AUTD3_API [[nodiscard]] float radian() const { return native_methods::AUTDPhaseToRad(*this); }

  AUTD3_API [[nodiscard]] uint8_t value() const noexcept { return _value; }

  auto operator<=>(const Phase&) const = default;  // LCOV_EXCL_LINE

  operator native_methods::Phase() const { return native_methods::Phase{_value}; }

  static constexpr Phase zero() noexcept { return Phase(0x00); }
  static constexpr Phase pi() noexcept { return Phase(0x80); }

 private:
  uint8_t _value;
};

}  // namespace autd3::driver
