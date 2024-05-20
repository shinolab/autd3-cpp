#include <gtest/gtest.h>

#include <autd3/driver/datagram/phase_filter.hpp>
#include <ranges>

#include "utils.hpp"

TEST(DriverDatagram, PhaseFilter) {
  auto autd = create_controller();

  autd.send(autd3::driver::PhaseFilter::additive([](const auto& dev) {
    return [&](const auto& tr) {
      return dev.idx() == 0 && tr.idx() == 0 ? autd3::driver::Phase(0x80)
                                             : autd3::driver::Phase(0);
    };
  }));

  {
    const auto filter = autd.link().phase_filter(0);
    ASSERT_EQ(0x80, filter[0].value());
    ASSERT_TRUE(std::ranges::all_of(filter | std::ranges::views::drop(1),
                                    [](auto& d) { return d.value() == 0; }));
  }
  {
    const auto filter = autd.link().phase_filter(1);
    ASSERT_TRUE(
        std::ranges::all_of(filter, [](auto& d) { return d.value() == 0; }));
  }
}
