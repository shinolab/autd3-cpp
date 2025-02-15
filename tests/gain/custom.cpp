#include <gtest/gtest.h>

#include <autd3/gain/custom.hpp>
#include <ranges>

#include "utils.hpp"

TEST(Gain, Coustom) {
  auto autd = create_controller();

  autd.send(autd3::gain::Custom([](const autd3::driver::geometry::Device& dev) -> auto {
    return [&](const autd3::driver::geometry::Transducer& tr) {
      if (dev.idx() == 0 && tr.idx() == 0) return autd3::driver::Drive{autd3::driver::Phase(0x90), autd3::driver::EmitIntensity(0x80)};
      if (dev.idx() == 1 && tr.idx() == 248) return autd3::driver::Drive{autd3::driver::Phase(0x91), autd3::driver::EmitIntensity(0x81)};
      return autd3::driver::Drive::null();
    };
  }));

  {
    auto drives = autd.link<autd3::link::Audit>().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_EQ(0x80, drives[0].intensity._0);
    ASSERT_EQ(0x90, drives[0].phase._0);
    ASSERT_TRUE(std::ranges::all_of(drives | std::ranges::views::drop(1), [](auto& d) { return d.intensity._0 == 0 && d.phase._0 == 0; }));
  }

  {
    auto drives = autd.link<autd3::link::Audit>().drives(1, autd3::native_methods::Segment::S0, 0);
    const auto idx = autd[1].num_transducers() - 1;
    ASSERT_EQ(0x81, drives[idx].intensity._0);
    ASSERT_EQ(0x91, drives[idx].phase._0);
    ASSERT_TRUE(std::ranges::all_of(drives | std::ranges::views::take(idx), [](auto& d) { return d.intensity._0 == 0 && d.phase._0 == 0; }));
  }
}
