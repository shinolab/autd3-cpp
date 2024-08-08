#include <gtest/gtest.h>

#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(Modulation, Sine) {
  auto autd = create_controller();

  {
    const auto m = autd3::modulation::Sine(150 * autd3::driver::Hz)
                       .with_intensity(std::numeric_limits<uint8_t>::max() / 2)
                       .with_offset(std::numeric_limits<uint8_t>::max() / 4)
                       .with_phase(autd3::driver::pi / 2 * autd3::driver::rad);
    ASSERT_EQ(std::numeric_limits<uint8_t>::max() / 2, m.intensity());
    ASSERT_EQ(std::numeric_limits<uint8_t>::max() / 4, m.offset());
    ASSERT_EQ(autd3::driver::pi / 2 * autd3::driver::rad, m.phase());
    ASSERT_EQ(10, m.sampling_config().division());
    autd.send(m);

    for (auto& dev : autd.geometry()) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{127, 125, 120, 111, 100, 87, 73, 58, 43, 30, 18, 9, 3, 0, 0, 4, 12, 22, 34, 48, 63, 78, 92,  104, 114, 122, 126,
                                      126, 123, 117, 108, 96,  83, 68, 53, 39, 26, 15, 6, 1, 0, 1, 6, 15, 26, 39, 53, 68, 83, 96,  108, 117, 123, 126,
                                      126, 122, 114, 104, 92,  78, 63, 48, 34, 22, 12, 4, 0, 0, 3, 9, 18, 30, 43, 58, 73, 87, 100, 111, 120, 125};
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }

  {
    const auto m = autd3::modulation::Sine(150 * autd3::driver::Hz)
                       .with_sampling_config(autd3::driver::SamplingConfig(20))
                       .with_loop_behavior(autd3::driver::LoopBehavior::Finite(10));
    ASSERT_EQ(20, m.sampling_config().division());
    autd.send(m);
    for (auto& dev : autd.geometry()) ASSERT_EQ(20, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, SineWithMode) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Sine::nearest(150.f * autd3::driver::Hz));

  for (auto& dev : autd.geometry()) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{127, 156, 184, 209, 229, 244, 253, 254, 249, 237, 220, 197, 171, 142,
                                    112, 83,  57,  34,  17,  5,   0,   1,   10,  25,  45,  70,  98};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
  }

  ASSERT_THROW(autd.send(autd3::modulation::Sine(100.1f * autd3::driver::Hz)), autd3::AUTDException);
  autd.send(autd3::modulation::Sine::nearest(100.1f * autd3::driver::Hz));
}

TEST(Modulation, SineDefault) {
  const autd3::modulation::Sine m(150.0f * autd3::driver::Hz);
  ASSERT_TRUE(AUTDModulationSineIsDefault(m.modulation_ptr()));
}
