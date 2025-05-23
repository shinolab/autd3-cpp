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

TEST(Link, RemoteTwinCAT) {
  auto link = autd3::link::RemoteTwinCAT("xxx.xxx.xxx.xxx.xxx.xxx",
                                         autd3::link::RemoteTwinCATOption{.server_ip = "127.0.0.1", .client_ams_net_id = "xxx.xxx.xxx.xxx.xxx.xxx"});

#ifdef RUN_LINK_REMOTE_TWINCAT
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
