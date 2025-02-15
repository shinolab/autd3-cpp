#pragma once

#include "autd3.hpp"

inline void focus_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  const autd3::Point3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);
  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}), autd3::Focus(center, autd3::FocusOption{})));
}
