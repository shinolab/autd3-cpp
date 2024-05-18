#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/loop_behavior.hpp>
#include <autd3/exception.hpp>

TEST(DriverFirmwareFPGA, LoopBehaviorInfinite) {
  const auto l = autd3::driver::LoopBehavior::infinite();
  ASSERT_EQ(0xFFFFFFFF, l.rep);
}

TEST(DriverFirmwareFPGA, LoopBehaviorFinite) {
  const auto l = autd3::driver::LoopBehavior::finite(2);
  ASSERT_EQ(1, l.rep);

  ASSERT_THROW(autd3::driver::LoopBehavior::finite(0), autd3::AUTDException);
}

TEST(DriverFirmwareFPGA, LoopBehaviorOnce) {
  const auto l = autd3::driver::LoopBehavior::once();
  ASSERT_EQ(0, l.rep);
}
