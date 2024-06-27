#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/link/audit.hpp>

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

TEST(Controller, Builder) {
  auto autd =
      autd3::controller::ControllerBuilder(std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());
  ASSERT_EQ(40000, autd.link().ultrasound_freq(0));
  autd.close();
}

#ifdef AUTD3_ASYNC_API
TEST(Controller, BuilderAsync) {
  auto autd = sync_wait(autd3::controller::ControllerBuilder(std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                            .open_async(autd3::link::Audit::builder()));
  ASSERT_EQ(40000, autd.link().ultrasound_freq(0));
  sync_wait(autd.close_async());
}
#endif
