#pragma once

#include "autd3.hpp"

template <typename L>
inline void focus_test(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer::default_();
  autd.send(silencer);

  auto m = autd3::modulation::Sine::create(150 * autd3::Hz);  // 150Hz AM

  const autd3::Vector3 center = autd.geometry().center() + autd3::Vector3(0.0, 0.0, 150.0);
  autd3::gain::Focus g(center);

  autd.send(m, g);
}
