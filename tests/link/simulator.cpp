// File: simulator.cpp
// Project: link
// Created Date: 26/09/2023
// Author: Shun Suzuki
// -----
// Last Modified: 02/12/2023
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2023 Shun Suzuki. All rights reserved.
//

#include <gtest/gtest.h>

#include <autd3/internal/controller.hpp>
#include <autd3/link/simulator.hpp>

TEST(Link, Simulator) {
  auto link = autd3::link::Simulator::builder(8080).with_server_ip("127.0.0.1").with_timeout(std::chrono::milliseconds(200));

#ifdef RUN_LINK_SIMULATOR
  auto autd = autd3::internal::ControllerBuilder()
                  .add_device(autd3::internal::geometry::AUTD3(autd3::internal::Vector3::Zero()))
                  .open_with_async(std::move(link))
                  .get();

  autd.close();
#else
  (void)link;
#endif
}
