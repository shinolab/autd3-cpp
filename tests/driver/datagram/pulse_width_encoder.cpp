#include <gtest/gtest.h>

#include <algorithm>
#include <autd3/driver/datagram/pulse_width_encoder.hpp>
#include <random>

#include "utils.hpp"

TEST(DriverDatagram, PulseWidthEncoder) {
  auto autd = create_controller();

  std::vector<uint16_t> buf(65536);

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_int_distribution<uint16_t> dist(0, 256);
  std::generate(buf.begin(), buf.end(), [&]() { return dist(engine); });
  std::sort(buf.begin(), buf.end());

  autd.send(autd3::driver::PulseWidthEncoder(buf));

  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(1));

  std::vector<uint16_t> buf_default(65536, 256);
  std::ranges::for_each(std::views::iota(0, 255 * 255),
                        [&](auto i) { buf_default[i] = std::round(std::asin(static_cast<double>(i) / 255. / 255.) / std::numbers::pi * 512); });
  autd.send(autd3::driver::PulseWidthEncoder());
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(1));
}
