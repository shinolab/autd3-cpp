// File: twincat.cpp
// Project: examples
// Created Date: 12/05/2022
// Author: Shun Suzuki
// -----
// Last Modified: 05/01/2024
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022-2023 Shun Suzuki. All rights reserved.
//

#include "autd3/link/twincat.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

coro::task<int> main_() {
  auto autd = co_await autd3::ControllerBuilder().add_device(autd3::AUTD3(autd3::Vector3::Zero())).open_with_async(autd3::link::TwinCAT::builder());
  auto res = co_await run(autd);
  co_return res;
}

int main() try { return sync_wait(main_()); } catch (std::exception& e) {
  print_err(e);
  return -1;
}
