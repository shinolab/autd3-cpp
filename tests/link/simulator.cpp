#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/simulator.hpp>

TEST(Link, Simulator) {
  auto link = autd3::link::Simulator::builder("127.0.0.1:8080");

#ifdef RUN_LINK_SIMULATOR
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Point3::origin())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
