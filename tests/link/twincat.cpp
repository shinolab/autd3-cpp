#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/twincat.hpp>

TEST(Link, TwinCAT) {
  auto link = autd3::link::TwinCAT::builder();

#ifdef RUN_LINK_TWINCAT
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Point3::origin())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}

TEST(Link, RemoteTwinCAT) {
  auto link =
      autd3::link::RemoteTwinCAT::builder("xxx.xxx.xxx.xxx.xxx.xxx").with_server_ip("127.0.0.1").with_client_ams_net_id("xxx.xxx.xxx.xxx.xxx.xxx");

#ifdef RUN_LINK_REMOTE_TWINCAT
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Point3::origin())}).open(std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
