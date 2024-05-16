#pragma once

#include "autd3/driver/defined/angle.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Phase final {
 public:
  explicit constexpr Phase(const uint8_t value) : _value(value) {}
  explicit Phase(const Angle value) : _value(native_methods::AUTDPhaseFromRad(value.to_radian())) {}

  [[nodiscard]] double radian() const { return native_methods::AUTDPhaseToRad(_value); }

  [[nodiscard]] uint8_t value() const noexcept { return _value; }

  auto operator<=>(const Phase&) const = default;

 private:
  uint8_t _value;
};

}  // namespace autd3::driver
