#include <gtest/gtest.h>

#include <autd3/modulation/cache.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(DriverDatagramModulation, Cache) {
  auto autd1 = create_controller();
  auto autd2 = create_controller();

  const auto m1 = autd3::modulation::Static(0x80);
  const auto m2 = autd3::modulation::Cache(autd3::modulation::Static(0x80));

  autd1.send(m1);
  autd2.send(m2);

  for (auto& dev : autd1) {
    auto mod = autd2.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    auto mod_expect = autd1.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect));
    ASSERT_EQ(0xFFFF, autd2.link<autd3::link::Audit>().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}
