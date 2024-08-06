#include <gtest/gtest.h>

#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/foci.hpp>
#include <autd3/driver/datagram/stm/gain.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Silencer) {
  auto autd = create_controller();

  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(10, autd.link().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(40, autd.link().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer::from_completion_time(std::chrono::microseconds(25 * 3), std::chrono::microseconds(25 * 4)));
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(3, autd.link().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(4, autd.link().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer::from_update_rate(10, 20));
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(10, autd.link().silencer_update_rate_intensity(dev.idx()));
    ASSERT_EQ(20, autd.link().silencer_update_rate_phase(dev.idx()));
    ASSERT_FALSE(autd.link().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer::disable());
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(1, autd.link().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(1, autd.link().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer());
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(10, autd.link().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(40, autd.link().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  ASSERT_TRUE(autd3::native_methods::AUTDDatagramSilencerFixedCompletionTimeIsDefault(autd3::driver::Silencer().ptr(autd.geometry())));
}

TEST(DriverDatagram, SilencerIsValid) {
  ASSERT_TRUE(autd3::driver::Silencer().is_valid(autd3::modulation::Sine(150u * autd3::driver::Hz)));
  ASSERT_TRUE(autd3::driver::Silencer().is_valid(
      autd3::driver::FociSTM<1>(1.0f * autd3::driver::Hz, std::vector{autd3::driver::Vector3::Zero(), autd3::driver::Vector3::Zero()})));
  ASSERT_TRUE(autd3::driver::Silencer().is_valid(autd3::driver::GainSTM(1.0f * autd3::driver::Hz, {autd3::gain::Null(), autd3::gain::Null()})));
}
