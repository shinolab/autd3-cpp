#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/nop.hpp>

#include "utils.hpp"

TEST(Controller, SleeperStd) {
  {
    const autd3::native_methods::SleeperWrap wrap = autd3::controller::StdSleeper();
    ASSERT_EQ(autd3::native_methods::SleeperTag::Std, wrap.tag);
    ASSERT_EQ(0u, wrap.value);
  }

  {
    const autd3::native_methods::SleeperWrap wrap = autd3::controller::StdSleeper{.timer_resolution = 1};
    ASSERT_EQ(autd3::native_methods::SleeperTag::Std, wrap.tag);
    ASSERT_EQ(1u, wrap.value);
  }
}

TEST(Controller, SleeperSpin) {
  const autd3::native_methods::SleeperWrap wrap =
      autd3::controller::SpinSleeper{.native_accuracy_ns = 1, .spin_strategy = autd3::native_methods::SpinStrategyTag::SpinLoopHint};
  ASSERT_EQ(autd3::native_methods::SleeperTag::Spin, wrap.tag);
  ASSERT_EQ(1u, wrap.value);
  ASSERT_EQ(autd3::native_methods::SpinStrategyTag::SpinLoopHint, wrap.spin_strategy);
}

TEST(Controller, SleeperWaitable) {
  const autd3::native_methods::SleeperWrap wrap = autd3::controller::WaitableSleeper{};
  ASSERT_EQ(autd3::native_methods::SleeperTag::Waitable, wrap.tag);
}
