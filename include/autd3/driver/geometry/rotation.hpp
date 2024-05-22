#pragma once

#include "autd3/driver/defined/angle.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class EulerAngles {
 public:
  AUTD3_API [[nodiscard]] static inline Quaternion ZYZ(const Angle& z1, const Angle& y, const Angle& z2) {
    double v[4];
    native_methods::AUTDRotationFromEulerZYZ(z1.radian(), y.radian(), z2.radian(), v);
    return {v[0], v[1], v[2], v[3]};
  }
};

}  // namespace autd3::driver::geometry
