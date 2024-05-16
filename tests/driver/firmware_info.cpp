#include <gtest/gtest.h>

#include <autd3/driver/firmware_version.hpp>
#include <autd3/exception.hpp>
#include <ranges>
#include <sstream>

#include "utils.hpp"

TEST(Internal, FirmwareVersion) {
  auto autd = create_controller();

  ASSERT_EQ("v6.1.0", autd3::driver::FirmwareVersion::latest_version());

  {
    const auto infos = autd.firmware_version();
    std::ranges::for_each(std::ranges::views::iota(0) | std::ranges::views::take(infos.size()), [&](auto i) {
      std::stringstream ss;
      ss << i;
      ss << ": CPU = v6.1.0, FPGA = v6.1.0 [Emulator]";
      ASSERT_EQ(ss.str(), infos[i].info());
      std::stringstream ssf;
      ssf << infos[i];
      ASSERT_EQ(ss.str(), ssf.str());
    });
  }

  {
    autd.link().break_down();
    ASSERT_THROW((void)autd.firmware_version(), autd3::AUTDException);
  }
}

TEST(Internal, FirmwareVersionAsync) {
  auto autd = create_controller();

  ASSERT_EQ("v6.1.0", autd3::driver::FirmwareVersion::latest_version());

  {
    const auto infos = sync_wait(autd.firmware_version_async());
    std::ranges::for_each(std::ranges::views::iota(0) | std::ranges::views::take(infos.size()), [&](auto i) {
      std::stringstream ss;
      ss << i;
      ss << ": CPU = v6.1.0, FPGA = v6.1.0 [Emulator]";
      ASSERT_EQ(ss.str(), infos[i].info());
    });
  }

  {
    autd.link().break_down();
    ASSERT_ANY_THROW((void)sync_wait(autd.firmware_version_async()));
  }
}
