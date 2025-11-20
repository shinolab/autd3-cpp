#pragma once

#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>

#include "autd3.hpp"

inline void flag_test(autd3::Controller& autd) {
  std::cout << "press any key to run fan..." << std::endl;
  std::cin.ignore();

  autd.send((autd3::ForceFan([](const auto&) { return true; }), autd3::ReadsFPGAState([](const auto&) { return true; })));

  bool fin = false;
  std::cout << "press any key stop checking FPGA status..." << std::endl;
  auto fin_signal = std::thread([&] {
    std::cin.ignore();
    fin = true;
  });

  const std::vector prompts = {'-', '/', '|', '\\'};
  size_t prompts_idx = 0;
  while (!fin) {
    const auto states = autd.fpga_state();
    std::cout << prompts[prompts_idx++ / 1000 % prompts.size()] << " FPGA Status...\n";
    for (auto&& [i, state] : states | std::ranges::views::enumerate)
      std::cout << std::format("\x1b[0K[{}]: Thermal sensor assert = {}", i,
                               state.has_value() ? std::to_string(state.value().is_thermal_assert()) : "-")
                << std::endl;
    std::cout << std::format("\x1b[{}A", states.size() + 1);
  }
  std::cout << "\x1b[1F\x1b[0J" << std::endl;

  if (fin_signal.joinable()) fin_signal.join();

  autd.send((autd3::ForceFan([](const auto&) { return false; }), autd3::ReadsFPGAState([](const auto&) { return false; })));
}
