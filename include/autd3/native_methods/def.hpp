#pragma once

#include "autd3/def.hpp"

namespace autd3::native_methods {

struct Vector3 {
  float x;
  float y;
  float z;

  operator driver::Vector3() const { return {x, y, z}; }
};

struct Quaternion {
  float x;
  float y;
  float z;
  float w;

  operator driver::Quaternion() const { return {w, x, y, z}; }
};

}  // namespace autd3::native_methods
