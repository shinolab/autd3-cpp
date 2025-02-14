#pragma once

#include <autd3.hpp>

inline void bessel_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  const autd3::Point3 apex = autd.center();
  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}),
             autd3::Bessel(apex, autd3::Vector3::UnitZ(), 13.0 / 180.0 * autd3::pi * autd3::rad, autd3::BesselOption{})));
}
