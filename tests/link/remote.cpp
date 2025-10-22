#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/link/remote.hpp>

TEST(Link, RemoteOption) {
  {
    const auto option = autd3::link::RemoteOption();
    const auto inner = static_cast<autd3::native_methods::RemoteOption>(option);
    EXPECT_FALSE(inner.timeout.has_value);
  }

  {
    const auto option = autd3::link::RemoteOption{
        .timeout = std::chrono::nanoseconds(0),
    };
    const auto inner = static_cast<autd3::native_methods::RemoteOption>(option);
    ASSERT_TRUE(inner.timeout.has_value);
    EXPECT_EQ(inner.timeout.value.nanos, 0);
  }
}

TEST(Link, Remote) {
  auto link = autd3::link::Remote{"127.0.0.1:8080"};

#ifdef RUN_LINK_REMOTE
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
