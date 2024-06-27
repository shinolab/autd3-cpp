#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/stm_sampling_config.hpp>

TEST(DriverFirmwareFPGA, SamplingConfigFreq) {
  const autd3::native_methods::STMSamplingConfigWrap f = autd3::driver::STMSamplingConfig::Freq(4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.tag, autd3::native_methods::STMSamplingConfigTag::Freq);
  ASSERT_EQ(f.value.freq, 4000.0f);
}
