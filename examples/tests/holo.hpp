#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "autd3.hpp"
#include "autd3/gain/holo.hpp"

inline void holo_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  std::cout << "[0]: GS" << std::endl;
  std::cout << "[1]: GSPAT" << std::endl;
  std::cout << "[2]: Naive" << std::endl;
  std::cout << "[3]: Greedy" << std::endl;
  std::cout << "[Others]: GS-PAT" << std::endl;
  std::cout << "Choose number: ";

  std::string in;
  size_t idx;
  getline(std::cin, in);
  std::stringstream s(in);
  if (const auto is_empty = in == "\n"; !(s >> idx) || idx >= 4 || is_empty) idx = 1;

  autd3::Sine m(150 * autd3::Hz, autd3::SineOption{});

  const autd3::Point3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);
  auto amp = 5e3 * autd3::gain::holo::Pa;
  std::vector<std::pair<autd3::Point3, autd3::gain::holo::Amplitude>> foci = {
      {center + autd3::Vector3(30.0, 0.0, 0.0), amp},
      {center - autd3::Vector3(30.0, 0.0, 0.0), amp},
  };
  switch (idx) {
    case 0:
      autd.send((m, autd3::gain::holo::GS(foci, autd3::gain::holo::GSOption{})));
      break;
    case 1:
      autd.send((m, autd3::gain::holo::GSPAT(foci, autd3::gain::holo::GSPATOption{})));
      break;
    case 2:
      autd.send((m, autd3::gain::holo::Naive(foci, autd3::gain::holo::NaiveOption{})));
      break;
    case 3:
      autd.send((m, autd3::gain::holo::Greedy(foci, autd3::gain::holo::GreedyOption{})));
      break;
    default:
      break;
  }
}
