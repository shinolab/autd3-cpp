#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/firmware/fpga/fpga_state.hpp>
#include <autd3/exception.hpp>

#include "utils.hpp"

TEST(DriverDatagram, FPGAState) {
  auto autd = create_controller();

  {
    for (const auto infos = autd.fpga_info(); auto info : infos) ASSERT_FALSE(info.has_value());
  }

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  {
    autd.link().assert_thermal_sensor(0);

    const auto infos = autd.fpga_info();
    ASSERT_TRUE(infos[0].value().is_thermal_assert());
    ASSERT_FALSE(infos[1].value().is_thermal_assert());
  }

  {
    autd.link().deassert_thermal_sensor(0);
    autd.link().assert_thermal_sensor(1);

    const auto infos = autd.fpga_info();
    ASSERT_FALSE(infos[0].value().is_thermal_assert());
    ASSERT_TRUE(infos[1].value().is_thermal_assert());
  }

  {
    autd.link().break_down();
    ASSERT_THROW((void)autd.fpga_info(), autd3::AUTDException);
  }
}
