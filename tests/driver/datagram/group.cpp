#include <gtest/gtest.h>

#include <autd3/driver/datagram/group.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>
#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Group) {
  auto autd = create_controller();

  autd.send(autd3::driver::Group(
      [](auto& dev) -> std::optional<size_t> { return dev.idx(); },
      std::unordered_map<size_t, std::shared_ptr<autd3::driver::Datagram>>{
          {1, std::make_shared<autd3::gain::Null>()},
          {0, std::make_shared<autd3::driver::DatagramTuple<autd3::modulation::Sine<autd3::driver::Freq<uint32_t>>, autd3::gain::Uniform>>(
                  autd3::modulation::Sine(150 * autd3::driver::Hz, autd3::modulation::SineOption{}),
                  autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase::ZERO})}}));

  {
    const auto m = autd.link<autd3::link::Audit>().modulation(0, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link<autd3::link::Audit>().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0; }));
  }
  {
    auto drives = autd.link<autd3::link::Audit>().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0; }));
  }
}
