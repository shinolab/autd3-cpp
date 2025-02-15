#pragma once

#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/link/audit.hpp>

static inline autd3::controller::Controller create_controller() {
  return autd3::controller::Controller::open({autd3::driver::AUTD3(), autd3::driver::AUTD3()}, autd3::link::Audit());
}

#define ASSERT_NEAR_VECTOR3(val1, val2, abs_error) \
  do {                                             \
    ASSERT_NEAR(val1.x(), val2.x(), abs_error);    \
    ASSERT_NEAR(val1.y(), val2.y(), abs_error);    \
    ASSERT_NEAR(val1.z(), val2.z(), abs_error);    \
  } while (false)
