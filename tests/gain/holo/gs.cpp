#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/gain/holo.hpp>
#include <autd3/gain/holo/gs.hpp>
#include <autd3/link/audit.hpp>
#include <utils.hpp>

TEST(GainHolo, GS) {
  auto autd = create_controller();

  const std::vector foci{std::make_pair(autd.center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa),
                         std::make_pair(autd.center() + autd3::driver::Vector3(-30, 0, 150), 5e3 * autd3::gain::holo::Pa)};
  const auto g = autd3::gain::holo::GS(
      foci, autd3::gain::holo::GSOption{.repeat = 100, .constraint = autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::Intensity(0x80))});

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase._0 != 0; }));
  }
}

TEST(GainHolo, GSDefault) { ASSERT_TRUE(autd3::native_methods::AUTDGainGSIsDefault(autd3::gain::holo::GSOption())); }
