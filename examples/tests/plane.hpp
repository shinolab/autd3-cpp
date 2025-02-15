#pragma once

#include "autd3.hpp"

inline void plane_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}), autd3::Plane(autd3::Vector3(0, 0, 1), autd3::PlaneOption{})));
}
