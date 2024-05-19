#include <gtest/gtest.h>

#include <autd3/modulation/fourier.hpp>
#include <ranges>

#include "utils.hpp"

TEST(Modulation, FourierExact) {
  auto autd = create_controller();

  std::vector f{200 * autd3::driver::Hz};
  auto m = (autd3::modulation::Sine(50 * autd3::driver::Hz) + autd3::modulation::Sine(100 * autd3::driver::Hz))
               .add_component(autd3::modulation::Sine(150 * autd3::driver::Hz))
               .add_components_from_iter(
                   f | std::ranges::views::transform([](const autd3::driver::Freq<uint32_t> x) { return autd3::modulation::Sine(x); })) +
           autd3::modulation::Sine(250 * autd3::driver::Hz);

  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        127, 156, 183, 205, 220, 227, 226, 219, 205, 188, 170, 153, 139, 129, 124, 123, 127, 133, 140, 147, 152, 155, 154, 151, 145, 138, 131,
        125, 120, 118, 119, 122, 127, 132, 137, 140, 141, 141, 137, 133, 127, 121, 116, 113, 112, 113, 117, 121, 127, 131, 134, 135, 133, 129,
        122, 115, 108, 102, 99,  99,  101, 106, 113, 120, 127, 130, 129, 124, 115, 100, 83,  65,  48,  35,  27,  26,  34,  48,  70,  97,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l.value() == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierExactFloat) {
  auto autd = create_controller();

  std::vector f{200.0 * autd3::driver::Hz};
  auto m = autd3::modulation::Sine(50.0 * autd3::driver::Hz) + autd3::modulation::Sine(100.0 * autd3::driver::Hz);
  m.add_component(autd3::modulation::Sine(150.0 * autd3::driver::Hz));
  m.add_components_from_iter(f | std::ranges::views::transform([](const autd3::driver::Freq<double> x) { return autd3::modulation::Sine(x); }));
  m += autd3::modulation::Sine(250.0 * autd3::driver::Hz);

  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        127, 156, 183, 205, 220, 227, 226, 219, 205, 188, 170, 153, 139, 129, 124, 123, 127, 133, 140, 147, 152, 155, 154, 151, 145, 138, 131,
        125, 120, 118, 119, 122, 127, 132, 137, 140, 141, 141, 137, 133, 127, 121, 116, 113, 112, 113, 117, 121, 127, 131, 134, 135, 133, 129,
        122, 115, 108, 102, 99,  99,  101, 106, 113, 120, 127, 130, 129, 124, 115, 100, 83,  65,  48,  35,  27,  26,  34,  48,  70,  97,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l.value() == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierNearest) {
  auto autd = create_controller();

  auto m =
      autd3::modulation::Sine::with_freq_nearest(50.0 * autd3::driver::Hz) + autd3::modulation::Sine::with_freq_nearest(100.0 * autd3::driver::Hz);
  autd.send(m);

  for (auto& dev : autd.geometry()) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
;    std::vector<uint8_t> mod_expect{127, 142, 156, 171, 184, 196, 207, 217, 225, 231, 236, 238, 239, 238, 235, 231, 225, 218, 209, 200,
                                    191, 180, 170, 160, 150, 141, 132, 124, 118, 112, 108, 105, 104, 103, 104, 106, 109, 113, 117, 122,
                                    127, 132, 136, 141, 145, 147, 149, 150, 150, 148, 146, 141, 136, 129, 121, 113, 104, 94,  83,  73,
                                    63,  53,  44,  36,  29,  23,  18,  15,  15,  15,  18,  22,  29,  36,  46,  57,  70,  83,  97,  112};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l.value() == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierInvalidMix) {
  auto autd = create_controller();

  auto m = autd3::modulation::Sine(50 * autd3::driver::Hz) + autd3::modulation::Sine(100.0 * autd3::driver::Hz);

  ASSERT_THROW(autd.send(m), autd3::AUTDException);
}
