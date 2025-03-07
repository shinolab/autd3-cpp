#include <gtest/gtest.h>

#include <autd3/driver/datagram/datagram.hpp>
#include <autd3/driver/geometry/device.hpp>
#include <ranges>

#include "utils.hpp"

TEST(DriverGeomtry, DeviceIdx) {
  auto autd = create_controller();
  ASSERT_EQ(autd[0].idx(), 0);
  ASSERT_EQ(autd[1].idx(), 1);
}

TEST(DriverGeomtry, DeviceSoundSpeed) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.sound_speed(), 340e3);
    dev.set_sound_speed(350e3);
    ASSERT_EQ(dev.sound_speed(), 350e3);
  }
}

TEST(DriverGeomtry, DeviceSoundSpeedFromTemp) {
  for (auto autd = create_controller(); auto& dev : autd) {
    dev.set_sound_speed_from_temp(15);
    ASSERT_EQ(dev.sound_speed(), 340.29525e3);
  }
}

TEST(DriverGeomtry, DeviceNumTransducers) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.num_transducers(), 249);
  }
}

TEST(DriverGeomtry, DeviceCenter) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_NEAR_VECTOR3(dev.center(), autd3::driver::Vector3(86.625267028808594f, 66.71319580078125f, 0), 1e-6);
  }
}

TEST(DriverGeomtry, DeviceEnable) {
  auto autd = create_controller();
  for (auto& dev : autd) ASSERT_TRUE(dev.enable());

  autd[0].set_enable(true);
  autd[1].set_enable(false);
  ASSERT_TRUE(autd[0].enable());
  ASSERT_FALSE(autd[1].enable());
}

TEST(DriverGeomtry, DeviceWavelength) {
  for (auto autd = create_controller(); auto& dev : autd) ASSERT_NEAR(dev.wavelength(), 8.5, 1e-6);
}

TEST(DriverGeomtry, DeviceWavenumber) {
  for (auto autd = create_controller(); auto& dev : autd) ASSERT_NEAR(dev.wavenumber(), 2 * autd3::driver::pi / 8.5, 1e-6);
}

TEST(DriverGeomtry, TransducerLocal) {
  for (auto autd = create_controller(); auto& dev : autd) {
    std::ranges::for_each(std::views::iota(0) | std::views::take(dev.num_transducers()), [&dev](auto i) { ASSERT_EQ(dev[i].idx(), i); });
  }
}

TEST(DriverGeomtry, DeviceRotation) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.rotation(), autd3::driver::Quaternion::Identity());
  }
}

TEST(DriverGeomtry, DeviceDirectionX) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.x_direction(), autd3::driver::Vector3::UnitX());
  }
}

TEST(DriverGeomtry, DeviceDirectionY) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.y_direction(), autd3::driver::Vector3::UnitY());
  }
}

TEST(DriverGeomtry, DeviceDirectionAxial) {
  for (auto autd = create_controller(); auto& dev : autd) {
    ASSERT_EQ(dev.axial_direction(), autd3::driver::Vector3::UnitZ());
  }
}
