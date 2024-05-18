#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/link/nop.hpp>

TEST(Link, Nop) {
  auto link = autd3::link::Nop::builder();

  auto autd = autd3::controller::ControllerBuilder().add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero())).open(std::move(link));

  autd.close();
}
