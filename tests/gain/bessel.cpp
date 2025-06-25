#include <gtest/gtest.h>

#include <autd3/gain/bessel.hpp>

#include "utils.hpp"

TEST(Gain, Bessel) {
  auto autd = create_controller();

  autd.send(
      autd3::gain::Bessel{autd.center(), autd3::driver::Vector3::UnitZ(), autd3::driver::pi / 4 * autd3::driver::rad,
                          autd3::gain::BesselOption{.intensity = autd3::driver::Intensity(0x80), .phase_offset = autd3::driver::Phase::zero()}});

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase._0 != 0; }));
  }
}

TEST(Gain, BesselDefault) { ASSERT_TRUE(autd3::native_methods::AUTDGainBesselIsDefault(autd3::gain::BesselOption())); }
