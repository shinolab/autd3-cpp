#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/drive.hpp>

TEST(DriverFirmwareFPGA, Drive) {
  constexpr auto phase = autd3::driver::Phase(0x90);
  constexpr auto intensity = autd3::driver::Intensity(0x91);

  constexpr autd3::driver::Drive d{.phase = phase, .intensity = intensity};
  ASSERT_EQ(phase, d.phase);
  ASSERT_EQ(intensity, d.intensity);
}
