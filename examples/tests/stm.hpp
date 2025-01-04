#pragma once

#include <autd3.hpp>
#include <ranges>

using namespace std::ranges::views;

template <typename L>
inline void focus_stm(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer::disable();
  autd.send(silencer);

  autd3::modulation::Static m;

  const autd3::Point3 center = autd.center() + autd3::Vector3(0, 0, 150);
  constexpr size_t points_num = 200;
  constexpr auto radius = 30.0f;
  autd3::FociSTM stm(1.0f * autd3::Hz, iota(0) | take(points_num) | transform([&](auto i) {
                                         const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
                                         autd3::Point3 p = center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0);
                                         return p;
                                       }));

  autd.send((m, stm));
}

template <typename L>
inline void gain_stm(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer::disable();
  autd.send(silencer);

  autd3::modulation::Static m;

  const autd3::Point3 center = autd.center() + autd3::Vector3(0, 0, 150);
  constexpr size_t points_num = 50;
  constexpr auto radius = 30.0f;
  autd3::GainSTM stm(1.0f * autd3::Hz, iota(0) | take(points_num) | transform([&](auto i) {
                                         const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
                                         return autd3::gain::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0));
                                       }));

  autd.send((m, stm));
}
