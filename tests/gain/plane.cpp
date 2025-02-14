#include <gtest/gtest.h>

#include <autd3/gain/plane.hpp>

#include "utils.hpp"

TEST(Gain, Plane) {
  auto autd = create_controller();

  autd.send(autd3::gain::Plane{autd3::driver::Vector3::UnitZ(), autd3::gain::PlaneOption{.intensity = autd3::driver::EmitIntensity(0x80),
                                                                                         .phase_offset = autd3::driver::Phase(0x81)}});

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0x81; }));
  }
}

TEST(Gain, PlaneDefault) { ASSERT_TRUE(autd3::native_methods::AUTDGainPlanelIsDefault(autd3::gain::PlaneOption())); }
