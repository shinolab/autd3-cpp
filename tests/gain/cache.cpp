#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/emit_intensity.hpp>
#include <autd3/gain/cache.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(DriverDatagramGain, Cache) {
  auto autd = create_controller();

  const auto g = autd3::gain::Cache(autd3::gain::Uniform{autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90)});

  autd.send(g);
  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0x90; }));
  }
}
