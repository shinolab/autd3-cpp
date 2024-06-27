#include <gtest/gtest.h>

#include <algorithm>
#include <autd3/driver/datagram/pulse_width_encoder.hpp>
#include <random>

#include "utils.hpp"

TEST(DriverDatagram, PulseWidthEncoder) {
  auto autd = create_controller();

  std::vector<uint16_t> buf(32768);

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_int_distribution<uint16_t> dist(0, 256);
  std::ranges::generate(buf, [&] { return dist(engine); });
  std::ranges::sort(buf);

  autd.send(autd3::driver::PulseWidthEncoder([&](const auto&) { return [&](auto i) { return buf[i]; }; }));

  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(1));

  std::vector<uint16_t> buf_default(32768, 256);
  std::ranges::for_each(std::views::iota(0, 255 * 255 / 2), [&](auto i) {
    buf_default[i] =
        static_cast<uint16_t>(std::roundf(std::asin(static_cast<float>(i) / static_cast<float>(255 * 255 / 2)) / std::numbers::pi_v<float> * 512.f));
  });
  autd.send(autd3::driver::PulseWidthEncoder());
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(1));
}
