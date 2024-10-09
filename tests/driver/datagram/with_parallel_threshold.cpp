#include <gtest/gtest.h>

#include <autd3/driver/datagram/with_parallel_threshold.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(DriverDatagram, WithParallelThreshold) {
  auto autd = autd3::controller::ControllerBuilder(
                  std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                  .with_parallel_threshold(0)
                  .open(autd3::link::Audit::builder());
  ASSERT_EQ(0xFFFF, autd.link().last_parallel_threshold());

  autd.send(autd3::gain::Null());
  ASSERT_EQ(0, autd.link().last_parallel_threshold());

  autd.send(autd3::modulation::Static());
  ASSERT_EQ(0xFFFF, autd.link().last_parallel_threshold());

  autd.send(autd3::modulation::Static().with_parallel_threshold(10));
  ASSERT_EQ(10, autd.link().last_parallel_threshold());

  autd.send((autd3::modulation::Static(), autd3::modulation::Static()).with_parallel_threshold(5));
  ASSERT_EQ(5, autd.link().last_parallel_threshold());
}
