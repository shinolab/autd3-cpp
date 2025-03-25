#include <gtest/gtest.h>

#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/foci.hpp>
#include <autd3/driver/datagram/stm/gain.hpp>
#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(DriverDatagram, Silencer) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(40, autd.link<autd3::link::Audit>().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link<autd3::link::Audit>().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer(
      autd3::driver::FixedCompletionTime{.intensity{std::chrono::microseconds(25 * 3)}, .phase{std::chrono::microseconds(25 * 4)}}));
  for (auto& dev : autd) {
    ASSERT_EQ(3, autd.link<autd3::link::Audit>().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(4, autd.link<autd3::link::Audit>().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link<autd3::link::Audit>().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer(autd3::driver::FixedUpdateRate{.intensity = 10, .phase = 20}));
  for (auto& dev : autd) {
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().silencer_update_rate_intensity(dev.idx()));
    ASSERT_EQ(20, autd.link<autd3::link::Audit>().silencer_update_rate_phase(dev.idx()));
    ASSERT_FALSE(autd.link<autd3::link::Audit>().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer::disable());
  for (auto& dev : autd) {
    ASSERT_EQ(1, autd.link<autd3::link::Audit>().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(1, autd.link<autd3::link::Audit>().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link<autd3::link::Audit>().silencer_fixed_completion_steps_mode(dev.idx()));
  }

  autd.send(autd3::driver::Silencer());
  for (auto& dev : autd) {
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().silencer_completion_steps_intensity(dev.idx()));
    ASSERT_EQ(40, autd.link<autd3::link::Audit>().silencer_completion_steps_phase(dev.idx()));
    ASSERT_TRUE(autd.link<autd3::link::Audit>().silencer_fixed_completion_steps_mode(dev.idx()));
  }
}

TEST(DriverDatagram, SilencerIsDefault) {
  const auto default_ = autd3::driver::Silencer();
  ASSERT_TRUE(autd3::native_methods::AUTDDatagramSilencerFixedCompletionStepsIsDefault(std::get<0>(default_.config)));
}
