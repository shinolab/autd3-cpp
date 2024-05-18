#include <gtest/gtest.h>

#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(Modulation, Static) {
  auto autd = create_controller();

  auto m = autd3::modulation::Static(autd3::driver::EmitIntensity(32));
  autd.send(m);
  for (auto& dev : autd.geometry()) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{32, 32};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l.value() == r; }));
    ASSERT_EQ(0xFFFFFFFF, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, StaticDefault) {
  auto autd = create_controller();
  const auto m = autd3::modulation::Static();
  ASSERT_TRUE(AUTDModulationStaticIsDefault(m.modulation_ptr(autd.geometry())));
}
