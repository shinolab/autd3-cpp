#include <gtest/gtest.h>

#include <algorithm>
#include <autd3/driver/datagram/pulse_width_encoder.hpp>
#include <random>

#include "utils.hpp"

TEST(DriverDatagram, PulseWidthEncoder) {
  auto autd = create_controller();

  std::vector<autd3::driver::PulseWidth> buf;
  buf.resize(256, autd3::driver::PulseWidth(0));

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_int_distribution<uint32_t> dist(0x00, 0x1FF);
  std::ranges::generate(buf, [&] { return autd3::driver::PulseWidth(dist(engine)); });

  autd.send(autd3::driver::PulseWidthEncoder(
      [&](const auto&) { return [&](const autd3::driver::Intensity i) { return autd3::driver::PulseWidth(buf[i.value()]); }; }));

  ASSERT_EQ(buf, autd.link<autd3::link::Audit>().pulse_width_encoder_table(0));
  ASSERT_EQ(buf, autd.link<autd3::link::Audit>().pulse_width_encoder_table(1));

  std::vector<autd3::driver::PulseWidth> buf_default;
  buf_default.resize(256, autd3::driver::PulseWidth(0));
  std::ranges::for_each(std::views::iota(0, 256), [&](auto i) {
    buf_default[i] = autd3::driver::PulseWidth(static_cast<uint32_t>(std::round(std::asin(static_cast<double>(i) / 255.) / std::numbers::pi * 512.)));
  });
  autd.send(autd3::driver::PulseWidthEncoder());
  ASSERT_EQ(buf_default, autd.link<autd3::link::Audit>().pulse_width_encoder_table(0));
  ASSERT_EQ(buf_default, autd.link<autd3::link::Audit>().pulse_width_encoder_table(1));
}
