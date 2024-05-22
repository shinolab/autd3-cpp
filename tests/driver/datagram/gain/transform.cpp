#include <gtest/gtest.h>

#include <autd3/driver/datagram/gain/transform.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(DriverDatagramGain, Transform) {
  auto autd = create_controller();

  autd.send(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))
                .with_phase(autd3::driver::Phase(128))
                .with_transform([](const autd3::driver::geometry::Device& dev) {
                  return [&](const autd3::driver::geometry::Transducer&, const autd3::driver::Drive d) {
                    if (dev.idx() == 0) {
                      return autd3::driver::Drive{autd3::driver::Phase(d.phase.value() + 32), d.intensity};
                    }
                    return autd3::driver::Drive{autd3::driver::Phase(d.phase.value() - 32), d.intensity};
                  };
                }));

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 128 + 32; }));
  }

  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 128 - 32; }));
  }
}

TEST(DriverDatagramGain, TransformCheckOnlyForEnabled) {
  auto autd = create_controller();
  autd.geometry()[0].set_enable(false);

  std::vector cnt(autd.geometry().num_devices(), false);
  autd.send(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))
                .with_phase(autd3::driver::Phase(0x90))
                .with_transform([&cnt](const autd3::driver::geometry::Device& dev) {
                  cnt[dev.idx()] = true;
                  return [](const autd3::driver::geometry::Transducer&, const autd3::driver::Drive d) { return d; };
                }));

  ASSERT_FALSE(cnt[0]);
  ASSERT_TRUE(cnt[1]);

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}
