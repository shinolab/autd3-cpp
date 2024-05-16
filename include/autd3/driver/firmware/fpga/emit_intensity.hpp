#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class EmitIntensity final {
 public:
  explicit constexpr EmitIntensity(const uint8_t value) noexcept : _value(value) {}

  [[nodiscard]] constexpr uint8_t value() const noexcept { return _value; }

  friend EmitIntensity operator/(EmitIntensity&& lhs, const int& rhs) { return EmitIntensity(static_cast<uint8_t>(lhs._value / rhs)); }
  auto operator<=>(const EmitIntensity&) const = default;

 private:
  uint8_t _value;
};

}  // namespace autd3::driver

#include <limits>

namespace std {
template <>
struct numeric_limits<autd3::driver::EmitIntensity> {
  static constexpr autd3::driver::EmitIntensity min() noexcept { return autd3::driver::EmitIntensity{0x00}; }

  static constexpr autd3::driver::EmitIntensity max() noexcept { return autd3::driver::EmitIntensity{0xFF}; }
};
}  // namespace std
