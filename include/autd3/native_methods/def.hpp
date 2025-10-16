#pragma once

#include "autd3/def.hpp"

namespace autd3::native_methods {

struct Vector3 {
  float x;
  float y;
  float z;

  operator driver::Vector3() const { return {x, y, z}; }
};

struct Point3 {
  float x;
  float y;
  float z;

  operator driver::Point3() const { return {x, y, z}; }
};

struct Quaternion {
  float w;
  float i;
  float j;
  float k;

  operator driver::Quaternion() const { return {w, i, j, k}; }
};

}  // namespace autd3::native_methods
