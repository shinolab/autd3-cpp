#pragma once

#include <autd3.hpp>

template <typename L>
inline void bessel_test(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  autd3::modulation::Sine m(150 * autd3::Hz);  // 150Hz AM

  const autd3::Point3 apex = autd.center();
  autd3::gain::Bessel g(apex, autd3::Vector3::UnitZ(), 13.0 / 180.0 * autd3::pi * autd3::rad);

  autd.send((m, g));
}
