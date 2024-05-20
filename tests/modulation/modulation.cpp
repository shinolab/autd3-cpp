#include <gtest/gtest.h>

#include <autd3/modulation/modulation.hpp>

#include "utils.hpp"

class BurstModulation final
    : public autd3::modulation::Modulation<BurstModulation> {
 public:
  AUTD3_API [[nodiscard]] std::vector<autd3::driver::EmitIntensity> calc(
      const autd3::driver::geometry::Geometry&) const override {
    std::vector buffer(
        10, std::numeric_limits<autd3::driver::EmitIntensity>::min());
    buffer[0] = std::numeric_limits<autd3::driver::EmitIntensity>::max();
    return buffer;
  }

  explicit BurstModulation() noexcept
      : Modulation(autd3::driver::SamplingConfig::Division(5120)) {}
};

TEST(DriverDatagramModulation, Modulation) {
  auto autd = create_controller();

  autd.send(BurstModulation());

  for (auto& dev : autd.geometry()) {
    auto mod =
        autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{255, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_TRUE(std::ranges::equal(
        mod, mod_expect,
        [](const auto& l, const auto& r) { return l.value() == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(
                        dev.idx(), autd3::native_methods::Segment::S0));
  }
}
