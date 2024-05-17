#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/gain/holo.hpp>
#include <autd3/gain/holo/sdp.hpp>
#include <autd3/link/audit.hpp>

TEST(GainHolo, SDP) {
  auto autd =
      autd3::controller::ControllerBuilder().add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero())).open(autd3::link::Audit::builder());

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  std::vector<double> p{-30};
  auto g = autd3::gain::holo::SDP(std::move(backend))
               .add_focus(autd.geometry().center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa)
               .add_foci_from_iter(p | std::ranges::views::transform([&](auto x) {
                                     autd3::driver::Vector3 p = autd.geometry().center() + autd3::driver::Vector3(x, 0, 150);
                                     return std::make_pair(p, 5e3 * autd3::gain::holo::Pa);
                                   }))
               .with_alpha(1e-3)
               .with_lambda(0.9)
               .with_repeat(10)
               .with_constraint(autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::EmitIntensity(0x80)));

  autd.send(g);

  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

TEST(GainHolo, SDPDefault) {
  auto autd =
      autd3::controller::ControllerBuilder().add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero())).open(autd3::link::Audit::builder());
  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  auto g = autd3::gain::holo::SDP(std::move(backend));
  ASSERT_TRUE(autd3::native_methods::AUTDGainSDPIsDefault(g.gain_ptr(autd.geometry())));
}

#ifdef RUN_BACKEND_CUDA

#include "autd3/gain/holo/backend_cuda.hpp"

TEST(GainHolo, SDP_CUDA) {
  auto autd =
      autd3::controller::ControllerBuilder().add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero())).open(autd3::link::Audit::builder());

  auto backend = std::make_shared<autd3::gain::holo::CUDABackend>();
  std::vector<double> p{-30};
  auto g = autd3::gain::holo::SDP(std::move(backend))
               .add_focus(autd.geometry().center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa)
               .add_foci_from_iter(p | std::ranges::views::transform([&](auto x) {
                                     autd3::driver::Vector3 p = autd.geometry().center() + autd3::driver::Vector3(x, 0, 150);
                                     return std::make_pair(p, 5e3 * autd3::gain::holo::Pa);
                                   }))
               .with_alpha(1e-3)
               .with_lambda(0.9)
               .with_repeat(10)
               .with_constraint(autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::EmitIntensity(0x80)));

  autd.send(g);

  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

#endif
