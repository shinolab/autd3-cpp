#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/gain.hpp>
#include <autd3/driver/datagram/with_finite_loop.hpp>
#include <autd3/driver/datagram/with_segment.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, GainSTMSamplingConfig) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  {
    const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                   autd3::gain::Uniform(autd3::driver::Intensity(0x00), autd3::driver::Phase(0x00))};
    auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(20000), autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }

  {
    const std::shared_ptr<autd3::driver::Gain> g1 =
        std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0));
    const std::shared_ptr<autd3::driver::Gain> g2 = std::make_shared<autd3::gain::Null>();
    auto stm = autd3::driver::GainSTM(std::vector{g1, g2}, autd3::driver::SamplingConfig(20000), autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }
}

TEST(DriverDatagramSTM, GainSTMExactFreq) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  {
    const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                   autd3::gain::Uniform(autd3::driver::Intensity(0x00), autd3::driver::Phase(0x00))};
    auto stm = autd3::driver::GainSTM(gains, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }

  {
    const std::shared_ptr<autd3::driver::Gain> g1 =
        std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0));
    const std::shared_ptr<autd3::driver::Gain> g2 = std::make_shared<autd3::gain::Null>();
    auto stm = autd3::driver::GainSTM(std::vector{g1, g2}, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }
}

TEST(DriverDatagramSTM, GainSTMNearestFreq) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                 autd3::gain::Uniform(autd3::driver::Intensity(0x00), autd3::driver::Phase(0x00))};
  auto stm = autd3::driver::GainSTM(gains, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption()).into_nearest();
  ASSERT_EQ(20000u, stm.sampling_config().divide());
  autd.send(stm);
  for (const auto& dev : autd) {
    ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
    ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
    drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
  }
}

TEST(DriverDatagramSTM, GainSTMExactPeriod) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  {
    const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                   autd3::gain::Uniform(autd3::driver::Intensity(0x00), autd3::driver::Phase(0x00))};
    auto stm = autd3::driver::GainSTM(gains, std::chrono::seconds(1), autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }

  {
    const std::shared_ptr<autd3::driver::Gain> g1 =
        std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0));
    const std::shared_ptr<autd3::driver::Gain> g2 = std::make_shared<autd3::gain::Null>();
    auto stm = autd3::driver::GainSTM(std::vector{g1, g2}, std::chrono::seconds(1), autd3::driver::GainSTMOption());
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

      auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
      drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
    }
  }
}

TEST(DriverDatagramSTM, GainSTMNearestPeriod) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                 autd3::gain::Uniform(autd3::driver::Intensity(0x00), autd3::driver::Phase(0x00))};
  auto stm = autd3::driver::GainSTM(gains, std::chrono::seconds(1), autd3::driver::GainSTMOption()).into_nearest();
  ASSERT_EQ(20000u, stm.sampling_config().divide());
  autd.send(stm);
  for (const auto& dev : autd) {
    ASSERT_TRUE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
    ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));

    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
    drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x00 && d.phase._0 == 0x00; }));
  }
}

TEST(DriverDatagramSTM, GainSTMPhaseIntensityFull) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                 autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0))};
  auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                    autd3::driver::GainSTMOption{
                                        .mode = autd3::native_methods::GainSTMMode::PhaseIntensityFull,
                                    });
  autd.send(stm);
  for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));

  for (const auto& dev : autd) {
    ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));

    drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0xF0; }));
  }
}

TEST(DriverDatagramSTM, GainSTMPhaseFull) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                 autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0))};
  auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                    autd3::driver::GainSTMOption{
                                        .mode = autd3::native_methods::GainSTMMode::PhaseFull,
                                    });
  autd.send(stm);
  for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));

  for (const auto& dev : autd) {
    ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xF0; }));

    drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xF0; }));
  }
}

TEST(DriverDatagramSTM, GainSTMPhaseHalf) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const auto gains = std::vector{autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0)),
                                 autd3::gain::Uniform(autd3::driver::Intensity(0x80), autd3::driver::Phase(0xF0))};
  auto stm = autd3::driver::GainSTM(gains, autd3::driver::SamplingConfig(1),
                                    autd3::driver::GainSTMOption{
                                        .mode = autd3::native_methods::GainSTMMode::PhaseHalf,
                                    });
  autd.send(stm);
  for (const auto& dev : autd) ASSERT_EQ(1, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));

  for (const auto& dev : autd) {
    ASSERT_EQ(2u, autd.link<autd3::link::Audit>().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));

    drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0xFF && d.phase._0 == 0xFF; }));
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

  auto stm = autd3::driver::GainSTM(std::vector{autd3::gain::Null(), autd3::gain::Null()}, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption{});

  autd.send(stm);
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::WithSegment(stm, Segment::S1, autd3::driver::transition_mode::Immediate()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::WithSegment(stm, Segment::S0, autd3::driver::transition_mode::Later()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegmentGainSTM(Segment::S0, autd3::driver::transition_mode::Immediate()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }
}

TEST(DriverDatagramSTM, GainSTMLoopBehavior) {
  auto autd = create_controller();

  autd.send(autd3::driver::WithFiniteLoop(
      autd3::driver::GainSTM(std::vector{autd3::gain::Null(), autd3::gain::Null()}, 1.0f * autd3::driver::Hz, autd3::driver::GainSTMOption{}), 1,
      Segment::S1, autd3::driver::transition_mode::SyncIdx()));
  for (auto& dev : autd) ASSERT_EQ(0, autd.link<autd3::link::Audit>().stm_loop_count(dev.idx(), Segment::S1));
}
