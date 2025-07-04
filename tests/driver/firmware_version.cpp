#include <gtest/gtest.h>

#include <autd3/driver/firmware_version.hpp>
#include <autd3/exception.hpp>
#include <ranges>
#include <sstream>

#include "utils.hpp"

TEST(Driver, FirmwareVersion) {
  auto autd = create_controller();

  ASSERT_EQ("v12.1.0", autd3::driver::FirmwareVersion::latest_version());

  {
    const auto infos = autd.firmware_version();
    std::ranges::for_each(std::ranges::views::iota(0) | std::ranges::views::take(infos.size()), [&](auto i) {
      std::stringstream ss;
      ss << i;
      ss << ": CPU = v12.1.0, FPGA = v12.1.0 [Emulator]";
      ASSERT_EQ(ss.str(), infos[i].info());
      std::stringstream ssf;
      ssf << infos[i];
      ASSERT_EQ(ss.str(), ssf.str());
    });
  }

  {
    autd.link<autd3::link::Audit>().break_down();
    ASSERT_THROW((void)autd.firmware_version(), autd3::AUTDException);
    autd.link<autd3::link::Audit>().repair();
  }
}
