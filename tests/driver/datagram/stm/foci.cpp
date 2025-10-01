#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/foci.hpp>
#include <autd3/driver/datagram/with_finite_loop.hpp>
#include <autd3/driver/datagram/with_segment.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSTM, FocusSTMSampling) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  const autd3::driver::ControlPoint point{.point = center, .phase_offset = autd3::driver::Phase::zero()};
  const std::vector foci_point3 = {center, center};
  const std::vector foci_control_point = {point, point};

  {
    const auto stm = autd3::driver::FociSTM(foci_point3, autd3::driver::SamplingConfig(1));
    ASSERT_EQ(1u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(1u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }

  {
    const auto stm = autd3::driver::FociSTM(foci_control_point, autd3::driver::SamplingConfig(1));
    ASSERT_EQ(1u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(1u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }

  {
    const auto stm = autd3::driver::FociSTM(foci_point3, 1.0f * autd3::driver::Hz);
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }

  {
    const auto stm = autd3::driver::FociSTM(foci_control_point, 1.0f * autd3::driver::Hz);
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }

  {
    const auto stm = autd3::driver::FociSTM(foci_point3, std::chrono::seconds(1));
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }

  {
    const auto stm = autd3::driver::FociSTM(foci_control_point, std::chrono::seconds(1));
    ASSERT_EQ(20000u, stm.sampling_config().divide());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link<autd3::link::Audit>().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
      ASSERT_EQ(20000u, autd.link<autd3::link::Audit>().stm_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, 0xFF);
    }
  }
}

TEST(DriverDatagramSTM, FociSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }

  const autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  const std::vector foci = {center, center};

  const auto stm = autd3::driver::FociSTM(foci, 1.0f * autd3::driver::Hz);

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

  autd.send(autd3::driver::SwapSegment::FociSTM(Segment::S0, autd3::driver::transition_mode::Immediate()));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link<autd3::link::Audit>().current_stm_segment(dev.idx()));
  }
}

TEST(DriverDatagramSTM, FociSTMLoopBehavior) {
  auto autd = create_controller();

  const autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  const std::vector foci = {center, center};

  autd.send(autd3::driver::WithFiniteLoop(autd3::driver::FociSTM(foci, 1.0f * autd3::driver::Hz), 1, Segment::S1,
                                          autd3::driver::transition_mode::SyncIdx()));
  for (auto& dev : autd) ASSERT_EQ(0, autd.link<autd3::link::Audit>().stm_loop_count(dev.idx(), Segment::S1));
}

template <size_t N>
void test_foci_stm_n() {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  const autd3::driver::Point3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);

  std::array<autd3::driver::ControlPoint, N> points;
  points.fill(autd3::driver::ControlPoint{.point = center, .phase_offset = autd3::driver::Phase::zero()});
  {
    const auto stm = autd3::driver::FociSTM(std::vector{autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(0)},
                                                        autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(1)}},
                                            1.0f * autd3::driver::Hz);
    ASSERT_EQ(20000u, stm.sampling_config().divide());

    autd.send(stm);
    for (const auto& dev : autd)
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, i);
  }

  {
    const auto stm = autd3::driver::FociSTM(std::vector{autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(0)},
                                                        autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(1)}},
                                            1.0f * autd3::driver::Hz)
                         .into_nearest();
    ASSERT_EQ(20000u, stm.sampling_config().divide());

    autd.send(stm);
    for (const auto& dev : autd)
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, i);
  }

  {
    const auto stm = autd3::driver::FociSTM(std::vector{autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(0)},
                                                        autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(1)}},
                                            std::chrono::seconds(1));
    ASSERT_EQ(20000u, stm.sampling_config().divide());

    autd.send(stm);
    for (const auto& dev : autd)
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, i);
  }

  {
    const auto stm = autd3::driver::FociSTM(std::vector{autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(0)},
                                                        autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(1)}},
                                            std::chrono::seconds(1))
                         .into_nearest();
    ASSERT_EQ(20000u, stm.sampling_config().divide());

    autd.send(stm);
    for (const auto& dev : autd)
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, i);
  }

  {
    const auto stm = autd3::driver::FociSTM(std::vector{autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(0)},
                                                        autd3::driver::ControlPoints<N>{.points = points, .intensity = autd3::driver::Intensity(1)}},
                                            autd3::driver::SamplingConfig(1));
    ASSERT_EQ(1u, stm.sampling_config().divide());

    autd.send(stm);
    for (const auto& dev : autd)
      for (int i = 0; i < 2; i++)
        for (const auto& [_, intensity] : autd.link<autd3::link::Audit>().drives(dev.idx(), Segment::S0, i)) ASSERT_EQ(intensity._0, i);
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
