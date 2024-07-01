#include <gtest/gtest.h>

#include <autd3/modulation/mixer.hpp>
#include <ranges>

#include "utils.hpp"

TEST(Modulation, MixerExact) {
  auto autd = create_controller();

  std::vector f{50, 100, 150, 200, 250};
  const auto m = autd3::modulation::Mixer(
      f | std::ranges::views::transform([](const uint32_t x) {
        return autd3::modulation::Sine(x * autd3::driver::Hz);
      }));

  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod =
        autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        7, 21, 46, 81, 115, 138, 137, 113, 75, 38, 13, 2, 0, 0, 0, 0,
        0, 1,  1,  0,  0,   0,   3,   4,   4,  3,  1,  0, 0, 0, 0, 0,
        0, 0,  0,  0,  0,   3,   6,   8,   7,  5,  2,  0, 0, 0, 0, 0,
        0, 1,  2,  2,  1,   0,   0,   0,   0,  0,  0,  0, 0, 0, 0, 0,
        0, 0,  0,  0,  0,   0,   0,   0,   0,  0,  0,  0, 0, 0, 0, 1};
    ASSERT_TRUE(std::ranges::equal(
        mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(
                        dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, MixerExactFloat) {
  auto autd = create_controller();

  std::vector f{50.f, 100.f, 150.f, 200.f, 250.f};
  const auto m = autd3::modulation::Mixer(
      f | std::ranges::views::transform([](const float x) {
        return autd3::modulation::Sine(x * autd3::driver::Hz);
      }));

  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod =
        autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        7, 21, 46, 81, 115, 138, 137, 113, 75, 38, 13, 2, 0, 0, 0, 0,
        0, 1,  1,  0,  0,   0,   3,   4,   4,  3,  1,  0, 0, 0, 0, 0,
        0, 0,  0,  0,  0,   3,   6,   8,   7,  5,  2,  0, 0, 0, 0, 0,
        0, 1,  2,  2,  1,   0,   0,   0,   0,  0,  0,  0, 0, 0, 0, 0,
        0, 0,  0,  0,  0,   0,   0,   0,   0,  0,  0,  0, 0, 0, 0, 1};
    ASSERT_TRUE(std::ranges::equal(
        mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(
                        dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, MixerNearest) {
  auto autd = create_controller();

  std::vector f{50.f, 100.f};
  const auto m = autd3::modulation::Mixer(
      f | std::ranges::views::transform([](const float x) {
        return autd3::modulation::Sine::from_freq_nearest(x *
                                                          autd3::driver::Hz);
      }));
  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod =
        autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        63,  78,  95,  113, 131, 149, 166, 183, 196, 208, 217, 222, 223, 223,
        217, 208, 196, 182, 164, 146, 127, 106, 87,  67,  50,  35,  22,  12,
        5,   0,   0,   0,   4,   9,   17,  25,  33,  42,  50,  57,  63,  67,
        69,  70,  69,  66,  62,  56,  50,  43,  37,  29,  23,  17,  11,  7,
        4,   2,   0,   0,   0,   0,   0,   0,   1,   1,   1,   0,   0,   0,
        0,   0,   1,   3,   6,   11,  17,  26,  36,  49};
    ASSERT_TRUE(std::ranges::equal(
        mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(
                        dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, MixerInvalidMix) {
  auto autd = create_controller();

  const auto m = autd3::modulation::Mixer(
      {autd3::modulation::Sine(50 * autd3::driver::Hz),
       autd3::modulation::Sine(100.0f * autd3::driver::Hz)});
  ASSERT_THROW(autd.send(m), autd3::AUTDException);
}
