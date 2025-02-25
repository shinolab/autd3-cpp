#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/firmware/fpga/fpga_state.hpp>
#include <autd3/exception.hpp>

#include "utils.hpp"

TEST(DriverDatagram, FPGAState) {
  auto autd = create_controller();

  {
    for (const auto infos = autd.fpga_state(); auto info : infos) ASSERT_FALSE(info.has_value());
  }

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  {
    autd.link<autd3::link::Audit>().assert_thermal_sensor(0);

    const auto infos = autd.fpga_state();
    ASSERT_TRUE(infos[0].value().is_thermal_assert());
    ASSERT_FALSE(infos[1].value().is_thermal_assert());
  }

  {
    autd.link<autd3::link::Audit>().deassert_thermal_sensor(0);
    autd.link<autd3::link::Audit>().assert_thermal_sensor(1);

    const auto infos = autd.fpga_state();
    ASSERT_FALSE(infos[0].value().is_thermal_assert());
    ASSERT_TRUE(infos[1].value().is_thermal_assert());
  }

  {
    autd.link<autd3::link::Audit>().break_down();
    ASSERT_THROW((void)autd.fpga_state(), autd3::AUTDException);
    autd.link<autd3::link::Audit>().repair();
  }
}
