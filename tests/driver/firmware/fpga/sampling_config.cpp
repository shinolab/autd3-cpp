#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/sampling_config.hpp>

TEST(DriverFirmwareFPGA, SamplingConfigFreq) {
  const autd3::driver::SamplingConfig f = 4000.0f * autd3::driver::Hz;
  ASSERT_EQ(f.divide(), 10);
  ASSERT_EQ(f.freq(), 4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.period(), std::chrono::microseconds(250));
}

TEST(DriverFirmwareFPGA, SamplingConfigFreqNearest) {
  const auto f = autd3::driver::SamplingConfig(4000.0f * autd3::driver::Hz).into_nearest();
  ASSERT_EQ(f.divide(), 10);
  ASSERT_EQ(f.freq(), 4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.period(), std::chrono::microseconds(250));
}

TEST(DriverFirmwareFPGA, SamplingConfigFreqDivide) {
  const autd3::driver::SamplingConfig f(10);
  ASSERT_EQ(f.divide(), 10);
  ASSERT_EQ(f.freq(), 4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.period(), std::chrono::microseconds(250));
}
