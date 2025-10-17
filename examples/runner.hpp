#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "autd3.hpp"
#include "tests/bessel.hpp"
#include "tests/custom.hpp"
#include "tests/flag.hpp"
#include "tests/focus.hpp"
#include "tests/group.hpp"
#include "tests/holo.hpp"
#include "tests/plane.hpp"
#include "tests/stm.hpp"

inline void run(autd3::Controller& autd) {
  using F = std::function<void(autd3::Controller&)>;
  std::vector<std::pair<F, std::string>> tests = {std::pair(F{focus_test}, "Single focus test"),
                                                  std::pair(F{bessel_test}, "Bessel beam test"),
                                                  std::pair(F{plane_test}, "Plane wave test"),
                                                  std::pair(F{focus_stm}, "FociSTM test"),
                                                  std::pair(F{gain_stm}, "GainSTM test"),
                                                  std::pair(F{holo_test}, "Multiple foci test"),
                                                  std::pair(F{flag_test}, "Flag test"),
                                                  std::pair(F{custom}, "Custom Gain test"),
                                                  std::pair(F{group_by_transducer_test}, "Group (by Transducer) test")};

  if (autd.num_devices() >= 2) tests.emplace_back(F{group_by_device_test}, "Group (by Device) test");

  const auto firm_infos = autd.firmware_version();
  std::cout << "======== AUTD3 firmware information ========" << std::endl;
  std::copy(firm_infos.begin(), firm_infos.end(), std::ostream_iterator<autd3::FirmwareVersion>(std::cout, "\n"));
  std::cout << "============================================" << std::endl;

  while (true) {
    size_t i = 0;
    std::transform(tests.begin(), tests.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
                   [&i](const auto& test) { return "[" + std::to_string(i++) + "]: " + test.second; });
    std::cout << "[Others]: finish." << std::endl;

    std::cout << "Choose number: ";
    std::string in;
    size_t idx;
    getline(std::cin, in);
    std::stringstream s(in);
    if (const auto is_empty = in == "\n"; !(s >> idx) || idx >= tests.size() || is_empty) break;

    tests[idx].first(autd);

    std::cout << "press any key to finish..." << std::endl;
    std::cin.ignore();

    std::cout << "finish." << std::endl;
    autd.send((autd3::gain::Null(), autd3::Silencer()));
  }

  autd.close();
}
