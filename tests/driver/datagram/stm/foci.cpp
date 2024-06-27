#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/foci.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, FociSTM) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  {
    auto stm = autd3::driver::FociSTM<1>::from_freq(
        1.0f * autd3::driver::Hz,
        std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::driver::ControlPoints<1>{center}; }));
    autd.send(stm);
    for (const auto& dev : autd.geometry()) {
      ASSERT_FALSE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    }

    for (const auto& dev : autd.geometry()) ASSERT_EQ(10240000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }

  {
    auto stm = autd3::driver::FociSTM<1>::from_freq_nearest(
        1.0f * autd3::driver::Hz,
        std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::driver::ControlPoints{std::array{center}}; }));
    autd.send(stm);
    for (const auto& dev : autd.geometry()) {
      ASSERT_FALSE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    }

    for (const auto& dev : autd.geometry()) ASSERT_EQ(10240000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }

  {
    auto stm = autd3::driver::FociSTM<1>::from_sampling_config(autd3::driver::SamplingConfig::Division(512),
                                                               std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto i) {
                                                                 return autd3::driver::ControlPoints<1>{center}.with_intensity(i);
                                                               }));

    autd.send(stm);
    for (const auto& dev : autd.geometry()) ASSERT_EQ(512u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

    for (const auto& dev : autd.geometry()) {
      ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
      auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() == 0x00; }));
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));

      drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() == 0x01; }));
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));
    }
  }
}

TEST(DriverDatagramSTM, FociSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);
  auto stm = autd3::driver::FociSTM<1>::from_freq(
      1.0f * autd3::driver::Hz,
      std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) { return autd3::driver::ControlPoints{std::array{center}}; }));

  autd.send(stm);
  infos = autd.fpga_state();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(stm.with_segment(Segment::S1, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::FociSTM<1>::from_freq(1.0f * autd3::driver::Hz,
                                                 std::views::iota(0) | std::views::take(2) |
                                                     std::views::transform([&](auto) { return autd3::driver::ControlPoints{std::array{center}}; }))
                .with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_state();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegment::FociSTM(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd.geometry()) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }
}

template <size_t N>
void test_foci_stm_n() {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.geometry().center() + autd3::driver::Vector3(0, 0, 150);

  constexpr auto size = 2;
  std::array<autd3::driver::Vector3, N> points;
  points.fill(center);
  auto stm = autd3::driver::FociSTM<N>::from_sampling_config(autd3::driver::SamplingConfig::Division(512),
                                                             std::views::iota(0) | std::views::take(size) | std::views::transform([&](auto i) {
                                                               return autd3::driver::ControlPoints<N>{points}.with_intensity(i);
                                                             }));

  autd.send(stm);
  for (const auto& dev : autd.geometry()) {
    for (int i = 0; i < size; i++) {
      for (const auto& d : autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, i)) ASSERT_EQ(d.intensity.value(), i);
    }
  }
}

TEST(DriverDatagramSTM, FociSTM_N1) { test_foci_stm_n<1>(); }
TEST(DriverDatagramSTM, FociSTM_N2) { test_foci_stm_n<2>(); }
TEST(DriverDatagramSTM, FociSTM_N3) { test_foci_stm_n<3>(); }
TEST(DriverDatagramSTM, FociSTM_N4) { test_foci_stm_n<4>(); }
TEST(DriverDatagramSTM, FociSTM_N5) { test_foci_stm_n<5>(); }
TEST(DriverDatagramSTM, FociSTM_N6) { test_foci_stm_n<6>(); }
TEST(DriverDatagramSTM, FociSTM_N7) { test_foci_stm_n<7>(); }
TEST(DriverDatagramSTM, FociSTM_N8) { test_foci_stm_n<8>(); }
