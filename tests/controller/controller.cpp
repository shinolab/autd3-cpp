#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>
#include <autd3/modulation/sine.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(Controller, SenderOptionIsDefault) { ASSERT_TRUE(autd3::native_methods::AUTDSenderOptionIsDefault(autd3::controller::SenderOption())); }

TEST(Controller, ControllerClose) {
  {
    auto autd = create_controller();
    ASSERT_TRUE(autd.link<autd3::link::Audit>().is_open());

    autd.close();
    autd.close();
  }

  {
    auto autd = create_controller();
    autd.link<autd3::link::Audit>().break_down();
    ASSERT_THROW(autd.close(), autd3::AUTDException);
  }
}

TEST(Controller, ControllerSendSingle) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.send(autd3::modulation::Static());
  for (auto& dev : autd) {
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.link<autd3::link::Audit>().break_down();
  ASSERT_THROW(autd.send(autd3::modulation::Static()), autd3::AUTDException);
  autd.link<autd3::link::Audit>().repair();
}

TEST(Controller, ControllerSendTuple) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0 && d.phase._0 == 0; }));
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.send((autd3::modulation::Static(0x80), autd3::gain::Uniform{autd3::driver::EmitIntensity(0x81), autd3::driver::Phase(0x82)}));
  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x81 && d.phase._0 == 0x82; }));
    auto m = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0x80; }));
  }
}
