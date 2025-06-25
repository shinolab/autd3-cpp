#pragma once

#include <autd3.hpp>
#include <autd3/gain/custom.hpp>

inline void custom(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}), autd3::gain::Custom([](const auto& dev) -> auto {
               return [&](const auto& tr) -> autd3::Drive {
                 if (dev.idx() == 0 && tr.idx() == 0)
                   return autd3::Drive{.phase = autd3::Phase(0), .intensity = std::numeric_limits<autd3::Intensity>::max()};
                 if (dev.idx() == 0 && tr.idx() == 248)
                   return autd3::Drive{.phase = autd3::Phase(0), .intensity = std::numeric_limits<autd3::Intensity>::max()};
                 return autd3::Drive::null();
               };
             })));
}
