#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/loop_behavior.hpp>
#include <autd3/exception.hpp>

TEST(DriverFirmwareFPGA, LoopBehaviorInfinite) {
  const autd3::native_methods::LoopBehavior loop_behavior = autd3::driver::LoopBehavior::Infinite();
  ASSERT_EQ(0xFFFF, loop_behavior.rep);
}

TEST(DriverFirmwareFPGA, LoopBehaviorFinite) {
  const autd3::native_methods::LoopBehavior loop_behavior = autd3::driver::LoopBehavior::Finite(2);
  ASSERT_EQ(1, loop_behavior.rep);

  ASSERT_THROW((void)autd3::driver::LoopBehavior::Finite(0), autd3::AUTDException);
}

TEST(DriverFirmwareFPGA, LoopBehaviorOnce) {
  const autd3::native_methods::LoopBehavior loop_behavior = autd3::driver::LoopBehavior::ONCE();
  ASSERT_EQ(0, loop_behavior.rep);
}
