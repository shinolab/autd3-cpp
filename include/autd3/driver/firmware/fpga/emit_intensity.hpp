#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct EmitIntensity final {
  template <std::integral T>
  AUTD3_API explicit constexpr EmitIntensity(const T value) noexcept : _value(static_cast<uint8_t>(value)) {}

  AUTD3_API [[nodiscard]] constexpr uint8_t value() const noexcept { return _value; }

  AUTD3_API friend EmitIntensity operator/(const EmitIntensity& lhs, const int& rhs) { return EmitIntensity(static_cast<uint8_t>(lhs._value / rhs)); }
  auto operator<=>(const EmitIntensity&) const = default;  // LCOV_EXCL_LINE

  operator native_methods::EmitIntensity() const { return native_methods::EmitIntensity{_value}; }

 private:
  uint8_t _value;
};

}  // namespace autd3::driver

#include <limits>

template <>
class std::numeric_limits<autd3::driver::EmitIntensity> {
 public:
  static constexpr autd3::driver::EmitIntensity min() noexcept { return autd3::driver::EmitIntensity(0x00); }
  static constexpr autd3::driver::EmitIntensity max() noexcept { return autd3::driver::EmitIntensity(0xFF); }
};
