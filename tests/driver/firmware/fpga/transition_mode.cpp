#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/transition_mode.hpp>

TEST(DriverFirmwareFPGA, TransitionModeSyncIdx) {
  const auto m = autd3::driver::TransitionMode::SyncIdx;
  ASSERT_EQ(m.tag, autd3::native_methods::TransitionModeTag::SyncIdx);
  ASSERT_EQ(m.value, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeSysTime) {
  const auto now = autd3::driver::DcSysTime::now();
  const auto m = autd3::driver::TransitionMode::SysTime(now);
  ASSERT_EQ(m.tag, autd3::native_methods::TransitionModeTag::SysTime);
  ASSERT_EQ(m.value, now.sys_time());
}

TEST(DriverFirmwareFPGA, TransitionModeGPIO) {
  const auto m = autd3::driver::TransitionMode::GPIO(autd3::native_methods::GPIOIn::I1);
  ASSERT_EQ(m.tag, autd3::native_methods::TransitionModeTag::Gpio);
  ASSERT_EQ(m.value, 1);
}

TEST(DriverFirmwareFPGA, TransitionModeExt) {
  const auto m = autd3::driver::TransitionMode::Ext;
  ASSERT_EQ(m.tag, autd3::native_methods::TransitionModeTag::Ext);
  ASSERT_EQ(m.value, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeImmediate) {
  const auto m = autd3::driver::TransitionMode::Immediate;
  ASSERT_EQ(m.tag, autd3::native_methods::TransitionModeTag::Immediate);
  ASSERT_EQ(m.value, 0);
}
