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

  // TODO

  autd.send(autd3::driver::PulseWidthEncoder());

  // TODO
}
