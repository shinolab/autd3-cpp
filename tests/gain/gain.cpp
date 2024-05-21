#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/emit_intensity.hpp>
#include <autd3/gain/gain.hpp>

#include "utils.hpp"

class Uniform final : public autd3::gain::Gain<Uniform> {
 public:
  explicit Uniform(const uint8_t intensity, const uint8_t phase, std::vector<bool>* cnt)
      : _intensity(autd3::driver::EmitIntensity(intensity)), _phase(phase), _cnt(cnt) {}
  Uniform() = delete;
  Uniform(const Uniform& obj) = default;             // LCOV_EXCL_LINE
  Uniform& operator=(const Uniform& obj) = default;  // LCOV_EXCL_LINE
  Uniform(Uniform&& obj) = default;                  // LCOV_EXCL_LINE
  Uniform& operator=(Uniform&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~Uniform() = default;                      // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] std::unordered_map<size_t, std::vector<autd3::driver::Drive>> calc(
      const autd3::driver::geometry::Geometry& geometry) const override {
    return transform(geometry, [&](const auto& dev) {
      _cnt->operator[](dev.idx()) = true;
      return [&](const auto&) { return autd3::driver::Drive{_phase, _intensity}; };
    });
  }

 private:
  autd3::driver::EmitIntensity _intensity;
  autd3::driver::Phase _phase;
  std::vector<bool>* _cnt;
};

TEST(DriverDatagramGain, Gain) {
  auto autd = create_controller();

  std::vector cnt(autd.geometry().num_devices(), false);
  autd.send(Uniform(0x80, 0x90, &cnt));

  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}

TEST(DriverDatagramGain, GainCheckOnlyForEnabled) {
  auto autd = create_controller();
  autd.geometry()[0].set_enable(false);

  std::vector check(autd.geometry().num_devices(), false);
  autd.send(Uniform(0x80, 0x90, &check));

  ASSERT_FALSE(check[0]);
  ASSERT_TRUE(check[1]);

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}
