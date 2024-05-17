#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/gain.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <autd3/gain/focus.hpp>
#include <autd3/gain/uniform.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, GainSTM) {
  auto autd = autd3::controller::ControllerBuilder()
                  .add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero()))
                  .add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero()))
                  .open(autd3::link::Audit::builder());

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  auto stm =
      autd3::driver::GainSTM::from_freq(1.0 * autd3::driver::Hz)
          .add_gains_from_iter(std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::gain::Focus(center); }));
  autd.send(stm);
  for (const auto& dev : autd.geometry()) ASSERT_TRUE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
  for (const auto& dev : autd.geometry()) ASSERT_EQ(10240000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

  stm = autd3::driver::GainSTM::from_sampling_config(autd3::driver::SamplingConfig::Division(512))
            .add_gain(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80)))
            .add_gain(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80)))
            .with_mode(autd3::native_methods::GainSTMMode::PhaseIntensityFull);
  autd.send(stm);
  for (const auto& dev : autd.geometry()) ASSERT_EQ(512u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

  for (const auto& dev : autd.geometry()) {
    ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));

    drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));
  }

  stm.with_mode(autd3::native_methods::GainSTMMode::PhaseFull);
  autd.send(stm);
  for (const auto& dev : autd.geometry()) {
    ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0xFF && d.phase.value() == 0; }));

    drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0xFF && d.phase.value() == 0; }));
  }

  stm.with_mode(autd3::native_methods::GainSTMMode::PhaseHalf);
  autd.send(stm);
  for (const auto& dev : autd.geometry()) {
    ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0xFF && d.phase.value() == 0; }));

    drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0xFF && d.phase.value() == 0; }));
  }
}

TEST(DriverDatagramSTM, GainSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  auto stm =
      autd3::driver::GainSTM::from_freq(1.0 * autd3::driver::Hz)
          .add_gains_from_iter(std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::gain::Focus(center); }));

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

  autd.send(
      autd3::driver::GainSTM::from_freq(1.0 * autd3::driver::Hz)
          .add_gains_from_iter(std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::gain::Focus(center); }))
          .with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegment::gain_stm(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_info();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }
}
