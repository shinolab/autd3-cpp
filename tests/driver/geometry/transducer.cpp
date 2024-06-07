#include <gtest/gtest.h>

#include <autd3/driver/geometry/transducer.hpp>
#include <ranges>

#include "utils.hpp"

TEST(DriverGeomtry, TransducerIdx) {
  for (auto autd = create_controller(); auto& dev : autd.geometry()) {
    std::ranges::for_each(std::views::iota(0) | std::views::take(dev.num_transducers()), [&dev](auto i) { ASSERT_EQ(dev[i].idx(), i); });
  }
}

TEST(DriverGeomtry, TransducerPosition) {
  auto autd = create_controller();

  ASSERT_EQ(autd.geometry()[0][0].position(), autd3::driver::Vector3(0, 0, 0));
  ASSERT_EQ(autd.geometry()[0][autd3::driver::AUTD3::NUM_TRANS_IN_UNIT - 1].position(), autd3::driver::Vector3(172.72, 132.08, 0));

  ASSERT_EQ(autd.geometry()[1][0].position(), autd3::driver::Vector3(0, 0, 0));
  ASSERT_EQ(autd.geometry()[1][autd3::driver::AUTD3::NUM_TRANS_IN_UNIT - 1].position(), autd3::driver::Vector3(172.72, 132.08, 0));
}
