#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>

TEST(Controller, StrategyFixedSchedule) {
  const autd3::native_methods::TimerStrategyWrap wrap = autd3::controller::FixedSchedule{};
  ASSERT_EQ(autd3::native_methods::TimerStrategyTag::FixedSchedule, wrap.tag);
  ASSERT_EQ(autd3::native_methods::SleeperWrap(autd3::controller::SpinSleeper{}).tag, wrap.sleep.tag);
  ASSERT_EQ(autd3::native_methods::SleeperWrap(autd3::controller::SpinSleeper{}).value, wrap.sleep.value);
  ASSERT_EQ(autd3::native_methods::SleeperWrap(autd3::controller::SpinSleeper{}).spin_strategy, wrap.sleep.spin_strategy);
}

TEST(Controller, StrategyFixedDelay) {
  const autd3::native_methods::TimerStrategyWrap wrap = autd3::controller::FixedDelay{.sleeper = autd3::controller::SpinWaitSleeper{}};
  ASSERT_EQ(autd3::native_methods::TimerStrategyTag::FixedDelay, wrap.tag);
  ASSERT_EQ(autd3::native_methods::SleeperWrap(autd3::controller::SpinWaitSleeper{}).tag, wrap.sleep.tag);
}
