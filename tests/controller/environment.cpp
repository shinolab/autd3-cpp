#include <gtest/gtest.h>

#include "utils.hpp"

TEST(DriverGeomtry, EnvironmentSoundSpeed) {
  auto autd = create_controller();
  ASSERT_EQ(autd.environment.sound_speed, 340e3);
  autd.environment.sound_speed = 350e3;
  ASSERT_EQ(autd.environment.sound_speed, 350e3);
}

TEST(DriverGeomtry, EnvironmentSoundSpeedFromTemp) {
  auto autd = create_controller();
  autd.environment.set_sound_speed_from_temp(15);
  ASSERT_EQ(autd.environment.sound_speed, 340.29525e3);
}

TEST(DriverGeomtry, EnvironmentWavelength) {
  auto autd = create_controller();
  ASSERT_NEAR(autd.environment.wavelength(), 8.5, 1e-6);
}

TEST(DriverGeomtry, EnvironmentWavenumber) {
  auto autd = create_controller();
  ASSERT_NEAR(autd.environment.wavenumber(), 2 * autd3::driver::pi / 8.5, 1e-6);
}
