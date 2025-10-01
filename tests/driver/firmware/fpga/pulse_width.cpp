
#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/pulse_width.hpp>
#include <cmath>

TEST(DriverFirmwareFPGA, PulseWidth) {
  for (int i = 0; i < 512; i++) {
    const auto pw = autd3::driver::PulseWidth(i);
    ASSERT_EQ(i, pw.pulse_width(512));
  }

  ASSERT_THROW((void)autd3::driver::PulseWidth(512).pulse_width(512), autd3::AUTDException);
}

TEST(DriverFirmwareFPGA, PulseWidthFromDuty) {
  for (int i = 0; i < 512; i++) {
    const auto pw = autd3::driver::PulseWidth::from_duty(static_cast<float>(i) / 512.0f);
    ASSERT_EQ(i, pw.pulse_width(512));
  }

  ASSERT_THROW(autd3::driver::PulseWidth::from_duty(-1.0f), autd3::AUTDException);
  ASSERT_THROW(autd3::driver::PulseWidth::from_duty(1.0f), autd3::AUTDException);
}
