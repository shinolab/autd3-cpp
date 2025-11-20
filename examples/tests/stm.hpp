#pragma once

#include <autd3.hpp>
#include <ranges>

using namespace std::ranges::views;

inline void focus_stm(autd3::Controller& autd) {
  autd.send(autd3::Silencer::disable());

  const autd3::Point3 center = autd.center() + autd3::Vector3(0, 0, 150);
  constexpr size_t points_num = 200;
  constexpr auto radius = 30.0f;
  auto foci = iota(0) | take(points_num) | transform([&](auto i) {
                const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
                autd3::Point3 p = center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0);
                return p;
              }) |
              std::ranges::to<std::vector<autd3::Point3>>();

  autd.send((autd3::Static{}, autd3::FociSTM(foci, 1.0f * autd3::Hz)));
}

inline void gain_stm(autd3::Controller& autd) {
  auto silencer = autd3::Silencer::disable();
  autd.send(silencer);

  const autd3::Point3 center = autd.center() + autd3::Vector3(0, 0, 150);
  constexpr size_t points_num = 50;
  constexpr auto radius = 30.0f;
  std::vector<autd3::Focus> gains =
      iota(0) | take(points_num) | transform([&](auto i) {
        const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
        return autd3::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0), autd3::FocusOption{});
      }) |
      std::ranges::to<std::vector<autd3::Focus>>();

  autd.send((autd3::Static{}, autd3::GainSTM(gains, 1.0f * autd3::Hz, autd3::GainSTMOption{})));
}
