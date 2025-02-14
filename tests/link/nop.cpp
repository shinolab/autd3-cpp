#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/nop.hpp>

TEST(Link, Nop) {
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, autd3::link::Nop{});

  autd.close();
}
