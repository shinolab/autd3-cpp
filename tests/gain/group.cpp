#include <gtest/gtest.h>

#include <autd3/gain/group.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(Gain, Group) {
  auto autd = create_controller();

  const auto cx = autd.center().x();

  autd.send(autd3::gain::GainGroup(
      [cx](const auto&) {
        return [cx](const auto& tr) -> std::optional<const char*> {
          if (tr.position().x() < cx) return "uniform";
          return "null";
        };
      },
      std::unordered_map<const char*, std::shared_ptr<autd3::driver::Gain>>{
          {"null", std::make_shared<autd3::gain::Null>()},
          {"uniform", std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90))}}));

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (auto& tr : dev) {
      if (tr.position().x() < cx) {
        ASSERT_EQ(0x80, drives[tr.idx()].intensity._0);
        ASSERT_EQ(0x90, drives[tr.idx()].phase._0);
      } else {
        ASSERT_EQ(0, drives[tr.idx()].intensity._0);
        ASSERT_EQ(0, drives[tr.idx()].phase._0);
      }
    }
  }
}

TEST(Gain, GroupNullopt) {
  auto autd = create_controller();

  const auto cx = autd.center().x();

  autd.send(autd3::gain::GainGroup(
      [cx](const auto&) {
        return [cx](const auto& tr) -> std::optional<const char*> {
          if (tr.position().x() < cx) return "uniform";
          return std::nullopt;
        };
      },
      std::unordered_map<const char*, std::shared_ptr<autd3::driver::Gain>>{
          {"uniform", std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90))}}));

  for (auto& dev : autd) {
    auto drives = autd.link<autd3::link::Audit>().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (auto& tr : dev) {
      if (tr.position().x() < cx) {
        ASSERT_EQ(0x80, drives[tr.idx()].intensity._0);
        ASSERT_EQ(0x90, drives[tr.idx()].phase._0);
      } else {
        ASSERT_EQ(0, drives[tr.idx()].intensity._0);
        ASSERT_EQ(0, drives[tr.idx()].phase._0);
      }
    }
  }
}

TEST(Gain, GroupUnkownKey) {
  auto autd = create_controller();

  bool caught_err = false;
  try {
    autd.send(
        autd3::gain::GainGroup([](const auto&) { return [](const auto&) -> std::optional<const char*> { return "null"; }; },
                               std::unordered_map<const char*, std::shared_ptr<autd3::driver::Gain>>{
                                   {"null", std::make_shared<autd3::gain::Null>()},
                                   {"uniform", std::make_shared<autd3::gain::Uniform>(autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90))}}));
  } catch (autd3::AUTDException& e) {
    caught_err = true;
    ASSERT_STREQ("Unknown group key", e.what());
  }

  if (!caught_err) FAIL();
}

TEST(Gain, GroupCheckOnlyForEnabled) {
  auto autd = create_controller();

  autd.send(autd3::gain::GainGroup(
      [](const auto& dev) {
        return [&dev](const auto&) -> std::optional<int> {
          if (dev.idx() == 0)
            return std::nullopt;
          else
            return 0;
        };
      },
      std::unordered_map<int, autd3::gain::Uniform>{{0, autd3::gain::Uniform{autd3::driver::Intensity(0x80), autd3::driver::Phase(0x90)}}}));

  {
    auto drives = autd.link<autd3::link::Audit>().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0 && d.phase._0 == 0; }));
  }
  {
    auto drives = autd.link<autd3::link::Audit>().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity._0 == 0x80 && d.phase._0 == 0x90; }));
  }
}
