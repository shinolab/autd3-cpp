#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/sampling_config.hpp>

TEST(DriverFirmwareFPGA, SamplingConfigFreq) {
  const autd3::native_methods::SamplingConfigWrap f = autd3::driver::SamplingConfig::Freq(4000 * autd3::driver::Hz);
  ASSERT_EQ(f.tag, autd3::native_methods::SamplingConfigTag::Freq);
  ASSERT_EQ(f.value.freq, 4000);
}

TEST(DriverFirmwareFPGA, SamplingConfigFreqNearest) {
  const autd3::native_methods::SamplingConfigWrap f = autd3::driver::SamplingConfig::FreqNearest(4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.tag, autd3::native_methods::SamplingConfigTag::FreqNearest);
  ASSERT_EQ(f.value.freq_nearest, 4000.0);
}

TEST(DriverFirmwareFPGA, SamplingConfigFreqDivision) {
  const autd3::native_methods::SamplingConfigWrap f = autd3::driver::SamplingConfig::Division(5120);
  ASSERT_EQ(f.tag, autd3::native_methods::SamplingConfigTag::Division);
  ASSERT_EQ(f.value.div, 5120);
}

TEST(DriverFirmwareFPGA, SamplingConfigFreqDivisionRaw) {
  const autd3::native_methods::SamplingConfigWrap f = autd3::driver::SamplingConfig::DivisionRaw(5120);
  ASSERT_EQ(f.tag, autd3::native_methods::SamplingConfigTag::DivisionRaw);
  ASSERT_EQ(f.value.div, 5120);
}
