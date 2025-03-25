#include <gtest/gtest.h>

#include <algorithm>
#include <autd3/driver/datagram/pulse_width_encoder.hpp>
#include <random>

#include "utils.hpp"

TEST(DriverDatagram, PulseWidthEncoder) {
  auto autd = create_controller();

  std::vector<uint16_t> buf(256);

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_int_distribution<uint16_t> dist(0x00, 0x1FF);
  std::ranges::generate(buf, [&] { return dist(engine); });

  autd.send(autd3::driver::PulseWidthEncoder(
      [&](const auto&) { return [&](const autd3::driver::EmitIntensity i) { return autd3::driver::PulseWidth(buf[i.value()]); }; }));

  ASSERT_EQ(buf, autd.link<autd3::link::Audit>().pulse_width_encoder_table(0));
  ASSERT_EQ(buf, autd.link<autd3::link::Audit>().pulse_width_encoder_table(1));

  std::vector<uint16_t> buf_default(256);
  std::ranges::for_each(std::views::iota(0, 256), [&](auto i) {
    buf_default[i] = static_cast<uint16_t>(std::round(std::asin(static_cast<double>(i) / 255.) / std::numbers::pi * 512.));
  });
  autd.send(autd3::driver::PulseWidthEncoder());
  ASSERT_EQ(buf_default, autd.link<autd3::link::Audit>().pulse_width_encoder_table(0));
  ASSERT_EQ(buf_default, autd.link<autd3::link::Audit>().pulse_width_encoder_table(1));
}
