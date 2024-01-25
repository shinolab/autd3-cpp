#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/link/audit.hpp>

#include "autd3/gain/holo.hpp"

TEST(Gain_Holo, GS) {
  auto autd = autd3::controller::ControllerBuilder()
                  .add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero()))
                  .open_with(autd3::link::Audit::builder());

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  std::vector<double> p{-30};
  auto g = autd3::gain::holo::GS(std::move(backend))
               .add_focus(autd.geometry().center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pascal)
               .add_foci_from_iter(p | std::ranges::views::transform([&](auto x) {
                                     autd3::driver::Vector3 p = autd.geometry().center() + autd3::driver::Vector3(x, 0, 150);
                                     return std::make_pair(p, 5e3 * autd3::gain::holo::Pascal);
                                   }))
               .with_repeat(100)
               .with_constraint(autd3::gain::holo::EmissionConstraint::uniform(0x80));

  ASSERT_TRUE(autd.send(g));

  for (auto& dev : autd.geometry()) {
    auto [intensities, phases] = autd.link().intensities_and_phases(dev.idx(), 0);
    ASSERT_TRUE(std::ranges::all_of(intensities, [](auto d) { return d == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(phases, [](auto p) { return p != 0; }));
  }
}

TEST(Gain_Holo, GSDefault) {
  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  auto g = autd3::gain::holo::GS(std::move(backend));
  ASSERT_EQ(g.repeat(), autd3::native_methods::AUTDGainHoloGSDefaultRepeat());
  ASSERT_TRUE(autd3::native_methods::AUTDGainHoloConstraintEq(g.constraint().ptr(), autd3::native_methods::AUTDGainHoloGSDefaultConstraint()));
}

#ifdef RUN_BACKEND_CUDA

#include "autd3/gain/holo/backend_cuda.hpp"

TEST(Gain_Holo, GS_CUDA) {
  auto autd = autd3::controller::ControllerBuilder()
                  .add_device(autd3::driver::AUTD3(autd3::driver::Vector3::Zero()))
                  .open_with(autd3::link::Audit::builder());

  auto backend = std::make_shared<autd3::gain::holo::CUDABackend>();
  std::vector<double> p{-30};
  auto g = autd3::gain::holo::GS(std::move(backend))
               .add_focus(autd.geometry().center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pascal)
               .add_foci_from_iter(p | std::ranges::views::transform([&](auto x) {
                                     autd3::driver::Vector3 p = autd.geometry().center() + autd3::driver::Vector3(x, 0, 150);
                                     return std::make_pair(p, 5e3 * autd3::gain::holo::Pascal);
                                   }))
               .with_repeat(100)
               .with_constraint(autd3::gain::holo::EmissionConstraint::uniform(0x80));

  ASSERT_TRUE(autd.send(g));

  for (auto& dev : autd.geometry()) {
    auto [intensities, phases] = autd.link().intensities_and_phases(dev.idx(), 0);
    ASSERT_TRUE(std::ranges::all_of(intensities, [](auto d) { return d == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(phases, [](auto p) { return p != 0; }));
  }
}

#endif
