#include <gtest/gtest.h>

#include <autd3/driver/datagram/modulation/cache.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/modulation/modulation.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(DriverDatagramModulation, Cache) {
  auto autd1 = create_controller();
  auto autd2 = create_controller();

  const auto m1 = autd3::modulation::Static::with_intensity(0x80);
  const auto m2 = autd3::modulation::Static::with_intensity(0x80).with_cache();

  autd1.send(m1);
  autd2.send(m2);

  ASSERT_TRUE(std::ranges::all_of(m2.buffer(), [](auto d) { return d.value() == 0x80; }));
  for (const auto& m : m2) ASSERT_EQ(0x80, m.value());
  std::ranges::for_each(m2.cbegin(), m2.cend(), [](const auto& m) { ASSERT_EQ(0x80, m.value()); });
  for (size_t i = 0; i < 2; i++) ASSERT_EQ(0x80, m2[i].value());
  for (auto& dev : autd1.geometry()) {
    auto mod = autd2.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    auto mod_expect = autd1.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect));
    ASSERT_EQ(0xFFFFFFFF, autd2.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

class ForModulationCacheTest final : public autd3::modulation::Modulation<ForModulationCacheTest> {
 public:
  AUTD3_API [[nodiscard]] std::vector<uint8_t> calc(const autd3::driver::geometry::Geometry& geometry) const override {
    ++*_cnt;
    return {std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max()};
  }

  explicit ForModulationCacheTest(size_t* cnt) noexcept : Modulation(autd3::driver::SamplingConfig::Division(5120)), _cnt(cnt) {}

 private:
  size_t* _cnt;
};

TEST(DriverDatagramModulation, CacheCheckOnce) {
  auto autd = create_controller();

  {
    size_t cnt = 0;
    ForModulationCacheTest m(&cnt);
    autd.send(m);
    ASSERT_EQ(cnt, 1);
    autd.send(m);
    ASSERT_EQ(cnt, 2);
  }

  {
    size_t cnt = 0;
    ForModulationCacheTest g(&cnt);
    auto gc = g.with_cache();
    autd.send(gc);
    ASSERT_EQ(cnt, 1);
    autd.send(gc);
    ASSERT_EQ(cnt, 1);
  }
}
