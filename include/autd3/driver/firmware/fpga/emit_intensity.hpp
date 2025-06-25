#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct Intensity final {
  template <std::integral T>
  AUTD3_API explicit constexpr Intensity(const T value) noexcept : _value(static_cast<uint8_t>(value)) {}

  AUTD3_API [[nodiscard]] constexpr uint8_t value() const noexcept { return _value; }

  AUTD3_API friend Intensity operator/(const Intensity& lhs, const int& rhs) { return Intensity(static_cast<uint8_t>(lhs._value / rhs)); }
  auto operator<=>(const Intensity&) const = default;  // LCOV_EXCL_LINE

  operator native_methods::Intensity() const { return native_methods::Intensity{_value}; }

 private:
  uint8_t _value;
};

}  // namespace autd3::driver

#include <limits>

template <>
class std::numeric_limits<autd3::driver::Intensity> {
 public:
  static constexpr autd3::driver::Intensity min() noexcept { return autd3::driver::Intensity(0x00); }
  static constexpr autd3::driver::Intensity max() noexcept { return autd3::driver::Intensity(0xFF); }
};
