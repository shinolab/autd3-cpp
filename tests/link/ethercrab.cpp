#include <gtest/gtest.h>

#include <autd3/link/ethercrab.hpp>
#include <sstream>

TEST(Link, TracingInit) { autd3::link::tracing_init(); }

TEST(Link, ThreadPriority) {
  (void)autd3::link::ThreadPriority::Max();
  (void)autd3::link::ThreadPriority::Min();
  (void)autd3::link::ThreadPriority::Crossplarform(0);
  (void)autd3::link::ThreadPriority::Crossplarform(99);
  autd3::native_methods::ThreadPriorityPtr min = static_cast<autd3::native_methods::ThreadPriorityPtr>(autd3::link::ThreadPriority::Min());
  (void)min;
  ASSERT_THROW((void)autd3::link::ThreadPriority::Crossplarform(100), std::invalid_argument);
}

TEST(Link, Status) {
  const auto lost = autd3::link::Status::Lost();
  const auto state_change = autd3::link::Status::StateChanged();
  const auto err = autd3::link::Status::Error();
  const auto resumed = autd3::link::Status::Resumed();

  ASSERT_EQ(lost, autd3::link::Status::Lost());
  ASSERT_EQ(state_change, autd3::link::Status::StateChanged());
  ASSERT_EQ(err, autd3::link::Status::Error());
  ASSERT_EQ(resumed, autd3::link::Status::Resumed());
  ASSERT_NE(lost, state_change);
  ASSERT_NE(lost, err);
  ASSERT_NE(lost, resumed);
  ASSERT_NE(state_change, err);
  ASSERT_NE(state_change, lost);
  ASSERT_NE(state_change, resumed);
  ASSERT_NE(err, lost);
  ASSERT_NE(err, state_change);
  ASSERT_NE(err, resumed);
  ASSERT_NE(resumed, lost);
  ASSERT_NE(resumed, state_change);
  ASSERT_NE(resumed, err);

  std::stringstream ss;
  ss << lost;
  ASSERT_EQ(ss.str(), "");
}

TEST(Link, EtherCrabIsDefault) { ASSERT_TRUE(autd3::native_methods::AUTDLinkEtherCrabIsDefault(autd3::link::EtherCrabOption{})); }

TEST(Link, EtherCrab) {
  auto link = autd3::link::EtherCrab([](const uint16_t, const autd3::link::Status) {}, autd3::link::EtherCrabOption{});
  (void)link;
}
