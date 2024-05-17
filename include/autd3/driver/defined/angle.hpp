#pragma once

#include <numbers>

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Angle;

class UnitRad {
  friend Angle operator*(double l, const UnitRad&);
};
class UnitDegree {
  friend Angle operator*(double l, const UnitDegree&);
};

constexpr UnitRad rad = UnitRad{};
constexpr UnitDegree deg = UnitDegree{};

class Angle {
 public:
  friend class UnitRad;
  friend class UnitDegree;

  AUTD3_API [[nodiscard]] double to_radian() const { return _value; }

  AUTD3_API friend Angle operator*(const double l, const UnitRad&) { return Angle(l); }
  AUTD3_API friend Angle operator*(const double l, const UnitDegree&) { return Angle(l / 180 * std::numbers::pi); }

  auto operator<=>(const Angle&) const = default;

 private:
  AUTD3_API explicit Angle(const double value) : _value(value) {}

  double _value;
};

}  // namespace autd3::driver
