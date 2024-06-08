#include <gtest/gtest.h>

#include <autd3/driver/datagram/with_timeout.hpp>
#include <autd3/gain/null.hpp>

#include "utils.hpp"

TEST(DriverDatagram, WithTimeout) {
  auto autd = autd3::controller::ControllerBuilder(
                  std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                  .open(autd3::link::Audit::builder().with_timeout(std::chrono::nanoseconds::zero()));

  ASSERT_EQ(std::chrono::nanoseconds::zero(), autd.link().timeout());
  ASSERT_EQ(std::chrono::milliseconds(200), autd.link().last_timeout());

  autd.send(autd3::gain::Null());
  ASSERT_EQ(std::nullopt, autd.link().last_timeout());

  autd.send(autd3::gain::Null().with_timeout(std::chrono::milliseconds(100)));
  ASSERT_EQ(std::chrono::milliseconds(100), autd.link().last_timeout());

  autd.send(autd3::gain::Null().with_timeout(std::chrono::milliseconds(100)), autd3::gain::Null().with_timeout(std::chrono::milliseconds(200)));
  ASSERT_EQ(std::chrono::milliseconds(200), autd.link().last_timeout());
}
