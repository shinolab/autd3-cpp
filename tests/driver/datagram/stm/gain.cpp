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

#include "autd3/driver/datagram/with_segment.hpp"
#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, GainSTM) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  {
    autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
    const auto gains = std::vector{autd3::gain::Focus(center, autd3::gain::FocusOption()), autd3::gain::Focus(center, autd3::gain::FocusOption())};
    {
      auto stm = autd3::driver::GainSTM(gains, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption());
      ASSERT_EQ(20000u, stm.sampling_config().division());
      autd.send(stm);
      for (const auto& dev : autd) ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      for (const auto& dev : autd)
        ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }

    {
      auto stm = autd3::driver::GainSTM(gains, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption()).into_nearest();
      autd.send(stm);
      for (const auto& dev : autd) ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      for (const auto& dev : autd)
        ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }

  {
    const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0xFF)),
                                   autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0xFF))};
    {
      auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                        autd3::driver::GainSTMOption{
                                            .mode = autd3::native_methods::GainSTMMode::PhaseIntensityFull,
                                        });
      autd.send(stm);
      for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

      for (const auto& dev : autd) {
        ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
        auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xFF; }));

        drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xFF; }));
      }
    }

    {
      auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                        autd3::driver::GainSTMOption{
                                            .mode = autd3::native_methods::GainSTMMode::PhaseFull,
                                        });
      autd.send(stm);
      for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

      for (const auto& dev : autd) {
        ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
        auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));

        drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));
      }
    }

    {
      auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                        autd3::driver::GainSTMOption{
                                            .mode = autd3::native_methods::GainSTMMode::PhaseHalf,
                                        });
      autd.send(stm);
      for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

      for (const auto& dev : autd) {
        ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
        auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));

        drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
        ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));
      }
    }
  }
}

TEST(DriverDatagramSTM, GainSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  const auto gains = std::vector{autd3::gain::Focus(center, autd3::gain::FocusOption()), autd3::gain::Focus(center, autd3::gain::FocusOption())};

  auto stm = autd3::driver::GainSTM(gains, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption{});

  autd.send(stm);
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::WithSegment(stm, Segment::S1, autd3::driver::TransitionMode::Immediate()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::WithSegment(stm, Segment::S0, std::nullopt));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegment::GainSTM(Segment::S0, autd3::driver::TransitionMode::Immediate()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }
}
