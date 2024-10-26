#include <gtest/gtest.h>

#include <autd3/driver/datagram/with_parallel_threshold.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(DriverDatagram, WithParallelThreshold) {
  auto autd = autd3::controller::ControllerBuilder(
                  {autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                  .open(autd3::link::Audit::builder());

  autd.send((autd3::gain::Null(), autd3::modulation::Static()).with_parallel_threshold(std::nullopt));
}
