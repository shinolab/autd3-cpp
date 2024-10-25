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

TEST(Link, SOEMIsDefault) {
  auto link = autd3::link::SOEM::builder();
  ASSERT_TRUE(autd3::native_methods::AUTDLinkSOEMIsDefault(
      link.buf_size(), static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(link.send_cycle()).count()),
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(link.sync0_cycle()).count()), link.sync_mode(),
      link.process_priority(), link.thread_priority(),
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(link.state_check_interval()).count()), link.timer_strategy(),
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(link.sync_tolerance()).count()),
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(link.sync_timeout()).count())));
}

TEST(Link, SOEM) {
  auto link = autd3::link::SOEM::builder()
                  .with_ifname("")
                  .with_buf_size(32)
                  .with_send_cycle(std::chrono::milliseconds(1))
                  .with_sync0_cycle(std::chrono::milliseconds(1))
                  .with_err_handler([](const uint16_t, const autd3::link::Status) {})
                  .with_timer_strategy(autd3::native_methods::TimerStrategy::SpinSleep)
                  .with_sync_tolerance(std::chrono::microseconds(1))
                  .with_sync_timeout(std::chrono::seconds(10))
                  .with_sync_mode(autd3::link::SyncMode::DC)
                  .with_state_check_interval(std::chrono::milliseconds(100))
                  .with_thread_priority(autd3::link::ThreadPriority::Max)
                  .with_process_priority(autd3::link::ProcessPriority::High);

#ifdef RUN_LINK_SOEM
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}

TEST(Link, RemoteSOEM) {
  auto link = autd3::link::RemoteSOEM::builder("127.0.0.1:8080");
#ifdef RUN_LINK_REMOTE_SOEM

  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
#endif
