#include <gtest/gtest.h>

#include <autd3/gain/focus.hpp>

#include "utils.hpp"

TEST(Gain, Focus) {
  auto autd = create_controller();

  autd.send(autd3::gain::Focus(autd.geometry().center() + 150 * autd3::driver::Vector3::UnitZ()).with_intensity(autd3::driver::EmitIntensity(0x80)));

  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

TEST(Gain, FocusDefault) {
  auto autd = create_controller();

  const auto g = autd3::gain::Focus(autd3::driver::Vector3::Zero());
  ASSERT_TRUE(autd3::native_methods::AUTDGainFocusIsDefault(g.gain_ptr(autd.geometry())));
}
