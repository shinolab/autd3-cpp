#pragma once

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct AUTD3 {
  static constexpr size_t NUM_TRANS_IN_UNIT = native_methods::NUM_TRANS_IN_UNIT;
  static constexpr size_t NUM_TRANS_X = native_methods::NUM_TRANS_IN_X;
  static constexpr size_t NUM_TRANS_Y = native_methods::NUM_TRANS_IN_Y;
  static constexpr float TRANS_SPACING = native_methods::TRANS_SPACING_MM;
  static constexpr float DEVICE_WIDTH = native_methods::DEVICE_WIDTH_MM;
  static constexpr float DEVICE_HEIGHT = native_methods::DEVICE_HEIGHT_MM;

  Point3 pos = Point3::origin();
  Quaternion rot = Quaternion::Identity();
};
}  // namespace autd3::driver
