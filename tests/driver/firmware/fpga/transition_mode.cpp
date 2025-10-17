#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/transition_mode.hpp>

TEST(DriverFirmwareFPGA, TransitionModeSyncIdx) {
  const autd3::native_methods::TransitionModeWrap wrap = autd3::driver::transition_mode::SyncIdx().inner();
  ASSERT_EQ(wrap.tag, autd3::native_methods::TransitionModeTag::SyncIdx);
  ASSERT_EQ(wrap.value.null, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeSysTime) {
  const auto now = autd3::driver::DcSysTime(1000);
  const autd3::native_methods::TransitionModeWrap wrap = autd3::driver::transition_mode::SysTime(now).inner();
  ASSERT_EQ(wrap.tag, autd3::native_methods::TransitionModeTag::SysTime);
  ASSERT_EQ(wrap.value.sys_time.dc_sys_time, now.sys_time());
}

TEST(DriverFirmwareFPGA, TransitionModeGPIO) {
  const autd3::native_methods::TransitionModeWrap wrap = autd3::driver::transition_mode::GPIO(autd3::native_methods::GPIOIn::I1).inner();
  ASSERT_EQ(wrap.tag, autd3::native_methods::TransitionModeTag::Gpio);
  ASSERT_EQ(wrap.value.gpio_in, autd3::native_methods::GPIOIn::I1);
}

TEST(DriverFirmwareFPGA, TransitionModeExt) {
  const autd3::native_methods::TransitionModeWrap wrap = autd3::driver::transition_mode::Ext().inner();
  ASSERT_EQ(wrap.tag, autd3::native_methods::TransitionModeTag::Ext);
  ASSERT_EQ(wrap.value.null, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeImmediate) {
  const autd3::native_methods::TransitionModeWrap wrap = autd3::driver::transition_mode::Immediate().inner();
  ASSERT_EQ(wrap.tag, autd3::native_methods::TransitionModeTag::Immediate);
  ASSERT_EQ(wrap.value.null, 0);
}
