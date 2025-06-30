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
