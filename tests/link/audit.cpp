#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/link/audit.hpp>
#include <chrono>

TEST(Link, Audit) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());

  ASSERT_EQ(std::chrono::nanoseconds(0), autd.link().timeout());

  autd.close();
}
