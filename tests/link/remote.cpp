#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/remote.hpp>

TEST(Link, Remote) {
  auto link = autd3::link::Remote{"127.0.0.1:8080"};

#ifdef RUN_LINK_REMOTE
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
