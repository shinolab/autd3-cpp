#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/twincat.hpp>

TEST(Link, TwinCAT) {
  auto link = autd3::link::TwinCAT{};

#ifdef RUN_LINK_TWINCAT
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}