#include <gtest/gtest.h>

#include <autd3/gain/bessel.hpp>

#include "utils.hpp"

TEST(Gain, Bessel) {
  auto autd = create_controller();

  autd.send(autd3::gain::Bessel(autd.center(), autd3::driver::Vector3::UnitZ(), autd3::driver::pi / 4 * autd3::driver::rad)
                .with_intensity(autd3::driver::EmitIntensity(0x80)));

  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

TEST(Gain, BesselDefault) {
  auto autd = create_controller();
  const auto g = autd3::gain::Bessel(autd3::driver::Point3::origin(), autd3::driver::Vector3::UnitZ(), 0 * autd3::driver::rad);
  ASSERT_TRUE(autd3::native_methods::AUTDGainBesselIsDefault(g.intensity().value(), g.phase_offset().value()));
}
