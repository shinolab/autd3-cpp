#include <gtest/gtest.h>

#include <autd3/driver/datagram/gpio_out.hpp>

#include "utils.hpp"

TEST(DriverDatagram, DebugOutputIdx) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x00, 0x00, 0x00, 0x00};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link<autd3::link::Audit>().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link<autd3::link::Audit>().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::GPIOOutputs(
      [](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> std::optional<autd3::driver::GPIOOutputType> {
        switch (gpio) {
          case autd3::native_methods::GPIOOut::O0:
            return std::nullopt;
          case autd3::native_methods::GPIOOut::O1:
            return autd3::driver::GPIOOutputType::BaseSignal;
          case autd3::native_methods::GPIOOut::O2:
            return autd3::driver::GPIOOutputType::Thermo;
          case autd3::native_methods::GPIOOut::O3:
            return autd3::driver::GPIOOutputType::ForceFan;
        }
        return std::nullopt;  // LCOV_EXCL_LINE
      }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x00, 0x01, 0x02, 0x03};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link<autd3::link::Audit>().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link<autd3::link::Audit>().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::GPIOOutputs(
      [](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> std::optional<autd3::driver::GPIOOutputType> {
        switch (gpio) {
          case autd3::native_methods::GPIOOut::O0:
            return autd3::driver::GPIOOutputType::Sync;
          case autd3::native_methods::GPIOOut::O1:
            return autd3::driver::GPIOOutputType::ModSegment;
          case autd3::native_methods::GPIOOut::O2:
            return autd3::driver::GPIOOutputType::ModIdx(0x01);
          case autd3::native_methods::GPIOOut::O3:
            return autd3::driver::GPIOOutputType::StmSegment;
        }
        return std::nullopt;  // LCOV_EXCL_LINE
      }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x10, 0x20, 0x21, 0x50};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0001, 0x0000};
    ASSERT_EQ(ty, autd.link<autd3::link::Audit>().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link<autd3::link::Audit>().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::GPIOOutputs(
      [](const autd3::driver::geometry::Device& dev, const autd3::native_methods::GPIOOut gpio) -> std::optional<autd3::driver::GPIOOutputType> {
        switch (gpio) {
          case autd3::native_methods::GPIOOut::O0:
            return autd3::driver::GPIOOutputType::StmIdx(0x02);
          case autd3::native_methods::GPIOOut::O1:
            return autd3::driver::GPIOOutputType::IsStmMode;
          case autd3::native_methods::GPIOOut::O2:
            return autd3::driver::GPIOOutputType::PwmOut(&dev[3]);
          case autd3::native_methods::GPIOOut::O3:
            return autd3::driver::GPIOOutputType::Direct(true);
        }
        return std::nullopt;  // LCOV_EXCL_LINE
      }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x51, 0x52, 0xE0, 0xF0};
    std::array<uint64_t, 4> value{0x0002, 0x0000, 0x0003, 0x0001};
    ASSERT_EQ(ty, autd.link<autd3::link::Audit>().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link<autd3::link::Audit>().debug_values(dev.idx()));
  }

  const auto sys_time = autd3::driver::DcSysTime(10000);
  autd.send(autd3::driver::GPIOOutputs(
      [sys_time](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> std::optional<autd3::driver::GPIOOutputType> {
        switch (gpio) {
          case autd3::native_methods::GPIOOut::O0:
            return autd3::driver::GPIOOutputType::SysTimeEq(sys_time);
          default:
            return std::nullopt;
        }
      }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x60, 0x00, 0x00, 0x00};
    std::array<uint64_t, 4> value{(sys_time.sys_time() / 3125) >> 3, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link<autd3::link::Audit>().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link<autd3::link::Audit>().debug_values(dev.idx()));
  }
}
