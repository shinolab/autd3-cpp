#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/link/audit.hpp>

TEST(Controller, Builder) {
  auto autd =
      autd3::controller::ControllerBuilder(std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());
  ASSERT_EQ(40000, autd.link().ultrasound_freq(0));
  autd.close();
}

TEST(Controller, BuilderWithUltrasoundFreq) {
  auto autd = autd3::controller::ControllerBuilder(std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                  .with_ultrasound_freq(41 * autd3::driver::kHz)
                  .open(autd3::link::Audit::builder());
  ASSERT_EQ(41000, autd.link().ultrasound_freq(0));
  autd.close();
}
