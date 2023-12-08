// File: soem.cpp
// Project: link
// Created Date: 26/09/2023
// Author: Shun Suzuki
// -----
// Last Modified: 02/12/2023
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2023 Shun Suzuki. All rights reserved.
//

#ifndef WIN32

#include <gtest/gtest.h>

#include <autd3/internal/controller.hpp>
#include <autd3/link/soem.hpp>

[[noreturn]] void test_soem_on_lost(const char* msg) {
  std::cerr << msg;
#ifdef __APPLE__
  exit(-1);
#else
  std::quick_exit(-1);
#endif
}

void test_soem_on_err(const char* msg) { std::cerr << msg; }

TEST(Link, SOEM) {
  auto link = autd3::link::SOEM::builder()
                  .with_ifname("")
                  .with_buf_size(32)
                  .with_send_cycle(2)
                  .with_sync0_cycle(2)
                  .with_on_lost(&test_soem_on_lost)
                  .with_on_err(&test_soem_on_err)
                  .with_timer_strategy(autd3::internal::native_methods::TimerStrategy::Sleep)
                  .with_sync_mode(autd3::internal::native_methods::SyncMode::FreeRun)
                  .with_state_check_interval(std::chrono::milliseconds(100))
                  .with_timeout(std::chrono::milliseconds(200));

#ifdef RUN_LINK_SOEM
  auto autd = autd3::internal::ControllerBuilder()
                  .add_device(autd3::internal::geometry::AUTD3(autd3::internal::Vector3::Zero()))
                  .open_with_async(std::move(link))
                  .get();

  autd.close();
#else
  (void)link;
#endif
}

TEST(Link, RemoteSOEM) {
  auto link = autd3::link::RemoteSOEM::builder("127.0.0.1:8080").with_timeout(std::chrono::milliseconds(200));
#ifdef RUN_LINK_REMOTE_SOEM

  auto autd = autd3::internal::ControllerBuilder()
                  .add_device(autd3::internal::geometry::AUTD3(autd3::internal::Vector3::Zero()))
                  .open_with_async(std::move(link))
                  .get();

  autd.close();
#else
  (void)link;
#endif
}
#endif
