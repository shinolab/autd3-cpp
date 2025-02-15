#pragma once

#include "autd3.hpp"

inline void group_by_device_test(autd3::Controller& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  const autd3::Point3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);

  autd.group_send(
      [](const autd3::Device& dev) -> std::optional<const char*> {
        if (dev.idx() == 0) {
          return "null";
        } else if (dev.idx() == 1) {
          return "focus";
        } else {
          return std::nullopt;
        }
      },
      std::unordered_map<const char*, std::shared_ptr<autd3::driver::Datagram>>{
          {"focus", std::make_shared<autd3::DatagramTuple<autd3::Sine<autd3::Freq<uint32_t>>, autd3::Focus>>(
                        autd3::Sine(150 * autd3::driver::Hz, autd3::SineOption{}), autd3::Focus{center, autd3::FocusOption{}})},
          {"null", std::make_shared<autd3::DatagramTuple<autd3::Static, autd3::Null>>(autd3::Static{}, autd3::Null{})}});
}

inline void group_by_transducer_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  const auto cx = autd.center().x();

  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}),
             autd3::gain::Group(
                 [&cx](const autd3::Device&) {
                   return [&cx](const autd3::Transducer& tr) -> std::optional<const char*> {
                     if (tr.position().x() < cx) return "focus";
                     return "null";
                   };
                 },
                 std::unordered_map<const char*, std::shared_ptr<autd3::Gain>>{
                     {"focus", std::make_shared<autd3::Focus>(autd.center() + autd3::Vector3(0, 0, 150), autd3::FocusOption{})},
                     {"null", std::make_shared<autd3::Null>()}})));
}
