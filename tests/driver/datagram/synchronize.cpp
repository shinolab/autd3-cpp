#include <gtest/gtest.h>

#include <autd3/driver/datagram/synchronize.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Synchronize) {
  auto autd = autd3::controller::ControllerBuilder(
                  {autd3::driver::AUTD3(autd3::driver::Point3::origin()), autd3::driver::AUTD3(autd3::driver::Point3::origin())})
                  .open(autd3::link::Audit::builder());

  autd.send(autd3::driver::Synchronize());
}
