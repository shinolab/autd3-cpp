#include <gtest/gtest.h>

#include <autd3/gain/focus.hpp>

#include "utils.hpp"

TEST(Gain, Focus) {
  auto autd = create_controller();
  autd.send(autd3::gain::Focus{autd.center() + 150 * autd3::driver::Vector3::UnitZ(),
                               autd3::gain::FocusOption{.intensity = autd3::driver::Intensity(0x80)}});

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase._0 != 0; }));
  }
}

TEST(Gain, FocusDefault) { ASSERT_TRUE(autd3::native_methods::AUTDGainFocusIsDefault(autd3::gain::FocusOption())); }
