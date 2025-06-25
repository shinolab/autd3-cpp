#include <gtest/gtest.h>

#include <autd3/gain/holo.hpp>
#include <autd3/gain/holo/constraint.hpp>

#include "utils.hpp"

TEST(GainHolo, ConstraintUniform) {
  auto autd = create_controller();

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  const std::vector<std::pair<autd3::driver::Point3, autd3::gain::holo::Amplitude> > foci{
      {autd.center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa},
      {autd.center() - autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa}};
  const auto g = autd3::gain::holo::Naive(
      foci, autd3::gain::holo::NaiveOption{.constraint = autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::Intensity(0x80))},
      std::move(backend));

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase._0 != 0; }));
  }
}

TEST(GainHolo, ConstraintNormalize) {
  auto autd = create_controller();

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  const std::vector<std::pair<autd3::driver::Point3, autd3::gain::holo::Amplitude> > foci{
      {autd.center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa},
      {autd.center() - autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa}};
  const auto g = autd3::gain::holo::Naive(foci, autd3::gain::holo::NaiveOption{.constraint = autd3::gain::holo::EmissionConstraint::Normalize()},
                                          std::move(backend));
  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity._0 != 0; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase._0 != 0; }));
  }
}

TEST(GainHolo, ConstraintClamp) {
  auto autd = create_controller();

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  const std::vector<std::pair<autd3::driver::Point3, autd3::gain::holo::Amplitude> > foci{
      {autd.center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa},
      {autd.center() - autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa}};
  const auto g = autd3::gain::holo::Naive(foci,
                                          autd3::gain::holo::NaiveOption{.constraint = autd3::gain::holo::EmissionConstraint::Clamp(
                                                                             autd3::driver::Intensity(67), autd3::driver::Intensity(85))},
                                          std::move(backend));

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return 67 <= d.intensity._0 && d.intensity._0 <= 85; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase._0 != 0; }));
  }
}

TEST(GainHolo, ConstraintMultiply) {
  auto autd = create_controller();

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  const std::vector<std::pair<autd3::driver::Point3, autd3::gain::holo::Amplitude> > foci{
      {autd.center() + autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa},
      {autd.center() - autd3::driver::Vector3(30, 0, 150), 5e3 * autd3::gain::holo::Pa}};
  const auto g = autd3::gain::holo::Naive(foci, autd3::gain::holo::NaiveOption{.constraint = autd3::gain::holo::EmissionConstraint::Multiply(0)},
                                          std::move(backend));

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase._0 != 0; }));
  }
}
