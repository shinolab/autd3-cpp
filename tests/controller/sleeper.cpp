#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>

TEST(Controller, SleeperStd) {
  const autd3::native_methods::SleeperWrap wrap = autd3::controller::StdSleeper();
  ASSERT_EQ(autd3::native_methods::SleeperTag::Std, wrap.tag);
  ASSERT_EQ(0u, wrap.value);
}

TEST(Controller, SleeperSpin) {
  const autd3::native_methods::SleeperWrap wrap =
      autd3::controller::SpinSleeper{.native_accuracy_ns = 1, .spin_strategy = autd3::native_methods::SpinStrategyTag::SpinLoopHint};
  ASSERT_EQ(autd3::native_methods::SleeperTag::Spin, wrap.tag);
  ASSERT_EQ(1u, wrap.value);
  ASSERT_EQ(autd3::native_methods::SpinStrategyTag::SpinLoopHint, wrap.spin_strategy);
}

TEST(Controller, SleeperSpinWait) {
  const autd3::native_methods::SleeperWrap wrap = autd3::controller::SpinWaitSleeper{};
  ASSERT_EQ(autd3::native_methods::SleeperTag::SpinWait, wrap.tag);
}
