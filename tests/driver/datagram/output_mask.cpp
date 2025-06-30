#include <gtest/gtest.h>

#include <autd3/driver/datagram/output_mask.hpp>
#include <autd3/driver/datagram/with_segment.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(DriverDatagram, OutputMask) {
  auto autd = create_controller();

  autd.send(autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase(0x81)});
  for (const auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (const auto& tr : dev) {
      ASSERT_EQ(drives[tr.idx()].intensity._0, 0x80);
      ASSERT_EQ(drives[tr.idx()].phase._0, 0x81);
    }
  }

  autd.send(autd3::driver::OutputMask([&](const auto& dev) { return [&](const auto& tr) { return false; }; }));
  for (const auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (const auto& tr : dev) {
      ASSERT_EQ(drives[tr.idx()].intensity._0, 0x00);
      ASSERT_EQ(drives[tr.idx()].phase._0, 0x81);
    }
  }

  autd.send(autd3::driver::WithSegment(autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase(0x81)},
                                       autd3::native_methods::Segment::S1, autd3::driver::TransitionMode::Immediate()));
  for (const auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S1, 0);
    for (const auto& tr : dev) {
      ASSERT_EQ(drives[tr.idx()].intensity._0, 0x80);
      ASSERT_EQ(drives[tr.idx()].phase._0, 0x81);
    }
  }

  autd.send(autd3::driver::WithSegment(autd3::driver::OutputMask([&](const auto& dev) { return [&](const auto& tr) { return false; }; }),
                                       autd3::native_methods::Segment::S1, std::nullopt));
  for (const auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S1, 0);
    for (const auto& tr : dev) {
      ASSERT_EQ(drives[tr.idx()].intensity._0, 0x00);
      ASSERT_EQ(drives[tr.idx()].phase._0, 0x81);
    }
  }
}
