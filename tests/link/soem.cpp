#ifndef WIN32

#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/link/soem.hpp>

TEST(Link, ThreadPriority) {
  (void)autd3::link::ThreadPriority::Max;
  (void)autd3::link::ThreadPriority::Min;
  (void)autd3::link::ThreadPriority::Crossplarform(0);
  (void)autd3::link::ThreadPriority::Crossplarform(99);
  ASSERT_THROW((void)autd3::link::ThreadPriority::Crossplarform(100), std::invalid_argument);
}

TEST(Link, SOEM) {
  auto link = autd3::link::SOEM::builder()
                  .with_ifname("")
                  .with_buf_size(32)
                  .with_send_cycle(2)
                  .with_sync0_cycle(2)
                  .with_err_handler([](const uint16_t, const autd3::link::Status, const std::string&) {})
                  .with_timer_strategy(autd3::native_methods::TimerStrategy::Sleep)
                  .with_sync_tolerance(std::chrono::microseconds(1))
                  .with_sync_timeout(std::chrono::seconds(10))
                  .with_sync_mode(autd3::link::SyncMode::DC)
                  .with_state_check_interval(std::chrono::milliseconds(100))
                  .with_thread_priority(autd3::link::ThreadPriority::Max)
#ifdef WIN32
                  .with_process_priority(autd3::link::ProcessPriority::High)
#endif
                  .with_timeout(std::chrono::milliseconds(200));

#ifdef RUN_LINK_SOEM
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}

TEST(Link, RemoteSOEM) {
  auto link = autd3::link::RemoteSOEM::builder("127.0.0.1:8080").with_timeout(std::chrono::milliseconds(200));
#ifdef RUN_LINK_REMOTE_SOEM

  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
#endif
