#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/modulation/custom.hpp>

#include "utils.hpp"

TEST(DriverDatagramModulation, Custom) {
  auto autd = create_controller();

  std::vector buffer(10, std::numeric_limits<uint8_t>::min());
  buffer[0] = std::numeric_limits<uint8_t>::max();

  autd.send(autd3::modulation::Custom(buffer, autd3::driver::SamplingConfig(5120)));

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{255, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(5120, autd.link<autd3::link::Audit>().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}
