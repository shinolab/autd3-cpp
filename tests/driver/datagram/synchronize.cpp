#include <gtest/gtest.h>

#include <autd3/driver/datagram/synchronize.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Synchronize) {
  auto autd = create_controller();
  autd.send(autd3::driver::Synchronize());
}
