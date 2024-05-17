#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/focus.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, FocusSTM) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  auto stm = autd3::driver::FocusSTM::from_freq(1.0 * autd3::driver::Hz)
                 .add_foci_from_iter(std::views::iota(0) | std::views::take(1) | std::views::transform([&](auto) {
                                       return autd3::driver::ControlPoint{center, std::numeric_limits<autd3::driver::EmitIntensity>::max()};
                                     }));
  stm.add_focus(autd3::driver::ControlPoint{center, std::numeric_limits<autd3::driver::EmitIntensity>::max()});
  autd.send(stm);
  for (const auto& dev : autd.geometry()) {
    ASSERT_FALSE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
  }

  for (const auto& dev : autd.geometry()) ASSERT_EQ(10240000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

  stm = autd3::driver::FocusSTM::from_sampling_config(autd3::driver::SamplingConfig::Division(512))
            .add_focus(autd3::driver::ControlPoint{center, std::numeric_limits<autd3::driver::EmitIntensity>::max()});
  stm.add_focus(center);
  autd.send(stm);
  for (const auto& dev : autd.geometry()) ASSERT_EQ(512u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

  for (const auto& dev : autd.geometry()) {
    ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() != 0; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));

    drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() != 0; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));
  }
}

TEST(DriverDatagramSTM, FocusSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  auto stm = autd3::driver::FocusSTM::from_freq(1.0 * autd3::driver::Hz)
                 .add_foci_from_iter(std::views::iota(0) | std::views::take(1) | std::views::transform([&](auto) {
                                       return autd3::driver::ControlPoint{center, std::numeric_limits<autd3::driver::EmitIntensity>::max()};
                                     }))
                 .add_focus(center);

  autd.send(stm);
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(stm.with_segment(Segment::S1, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::FocusSTM::from_freq(1.0 * autd3::driver::Hz)
                .add_foci_from_iter(std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) {
                                      return autd3::driver::ControlPoint{center, std::numeric_limits<autd3::driver::EmitIntensity>::max()};
                                    }))
                .with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegment::focus_stm(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }
}
