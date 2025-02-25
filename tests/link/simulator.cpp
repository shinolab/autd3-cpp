#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/simulator.hpp>

TEST(Link, Simulator) {
  auto link = autd3::link::Simulator{"127.0.0.1:8080"};

#ifdef RUN_LINK_SIMULATOR
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
