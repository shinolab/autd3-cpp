#include <gtest/gtest.h>

#include <autd3/modulation/square.hpp>

#include "utils.hpp"

TEST(Modulation, SquareExact) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Square(200 * autd3::driver::Hz, autd3::modulation::SquareOption{.low = 32, .high = 85, .duty = 0.1f}));

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{85, 85, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, SquareExactFloat) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Square(200.0f * autd3::driver::Hz, autd3::modulation::SquareOption{.low = 32, .high = 85, .duty = 0.1f}));

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{85, 85, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, SquareNearest) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Square(150.0f * autd3::driver::Hz, autd3::modulation::SquareOption{}).into_nearest());

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
  }

  ASSERT_THROW(autd.send(autd3::modulation::Square(100.1f * autd3::driver::Hz, autd3::modulation::SquareOption{})), autd3::AUTDException);
  autd.send(autd3::modulation::Square(100.1f * autd3::driver::Hz, autd3::modulation::SquareOption{}).into_nearest());
}

TEST(Modulation, SquareDefault) { ASSERT_TRUE(autd3::native_methods::AUTDModulationSquareIsDefault(autd3::modulation::SquareOption())); }
