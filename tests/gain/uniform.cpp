#include <gtest/gtest.h>

#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(Gain, Uniform) {
  auto autd = create_controller();

  autd.send(autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90)});

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0x90; }));
  }
}
