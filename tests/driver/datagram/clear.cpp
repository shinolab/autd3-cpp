#include <gtest/gtest.h>

#include <autd3/driver/datagram/clear.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Clear) {
  auto autd = create_controller();

  autd.send(autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90)});
  for (auto& dev : autd) {
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0x90; }));
  }

  autd.send(autd3::driver::Clear());
  for (auto& dev : autd) {
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0 && d.phase._0 == 0; }));
  }
}
