#include <gtest/gtest.h>

#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(Gain, Uniform) {
  auto autd = create_controller();

  autd.send(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80)).with_phase(autd3::driver::Phase(0x90)));

  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}

TEST(Gain, UniformDefault) {
  auto autd = create_controller();
  auto g = autd3::gain::Uniform(autd3::driver::EmitIntensity(0x00));
  ASSERT_TRUE(autd3::native_methods::AUTDGainUniformIsDefault(g.gain_ptr(autd.geometry())));
}
