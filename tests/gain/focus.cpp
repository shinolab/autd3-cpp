#include <gtest/gtest.h>

#include <autd3/gain/focus.hpp>

#include "utils.hpp"

TEST(Gain, Focus) {
  auto autd = create_controller();

  ASSERT_TRUE(autd.send(autd3::gain::Focus(autd.geometry().center() + 150 * autd3::driver::Vector3::UnitZ()).with_intensity(0x80)));

  for (auto& dev : autd.geometry()) {
    auto [intensities, phases] = autd.link().intensities_and_phases(dev.idx(), 0);
    ASSERT_TRUE(std::ranges::all_of(intensities, [](auto d) { return d == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(phases, [](auto p) { return p != 0; }));
  }
}

TEST(Gain, FocusDefault) {
  auto g = autd3::gain::Focus(autd3::driver::Vector3::Zero());
  ASSERT_EQ(g.intensity().value(), autd3::native_methods::AUTDGainFocusDefaultIntensity());
}
