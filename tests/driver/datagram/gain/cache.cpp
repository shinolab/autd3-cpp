#include <gtest/gtest.h>

#include <autd3/driver/datagram/gain/cache.hpp>
#include <autd3/driver/firmware/fpga/emit_intensity.hpp>
#include <autd3/gain/gain.hpp>
#include <autd3/gain/uniform.hpp>

#include "utils.hpp"

TEST(DriverDatagramGain, Cache) {
  auto autd = create_controller();

  const auto g = autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90)).with_cache();

  autd.send(g);
  for (auto& dev : autd.geometry()) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}

class ForCacheTest final : public autd3::gain::Gain<ForCacheTest> {
 public:
  ForCacheTest() = delete;
  ForCacheTest(const ForCacheTest& obj) = default;             // LCOV_EXCL_LINE
  ForCacheTest& operator=(const ForCacheTest& obj) = default;  // LCOV_EXCL_LINE
  ForCacheTest(ForCacheTest&& obj) = default;                  // LCOV_EXCL_LINE
  ForCacheTest& operator=(ForCacheTest&& obj) = default;       // LCOV_EXCL_LINE
  ~ForCacheTest() override = default;                          // LCOV_EXCL_LINE
  explicit ForCacheTest(size_t* cnt) : _cnt(cnt) {}

  AUTD3_API [[nodiscard]] autd3::gain::GainCalcResult calc(const autd3::driver::geometry::Geometry&) const override {
    ++*_cnt;
    return transform(
        [](const auto&) { return [](const auto&) { return autd3::driver::Drive{autd3::driver::Phase(0x90), autd3::driver::EmitIntensity(0x80)}; }; });
  }

 private:
  size_t* _cnt;
};

TEST(DriverDatagramGain, CacheCheckOnce) {
  auto autd = create_controller();

  {
    size_t cnt = 0;
    const ForCacheTest g(&cnt);
    autd.send(g);
    ASSERT_EQ(cnt, 1);
    autd.send(g);
    ASSERT_EQ(cnt, 2);
  }

  {
    size_t cnt = 0;
    ForCacheTest g(&cnt);
    const auto gc = g.with_cache();
    ASSERT_EQ(cnt, 0);
    autd.send(gc);
    ASSERT_EQ(cnt, 1);
    autd.send(gc);
    ASSERT_EQ(cnt, 1);
  }
}

TEST(DriverDatagramGain, CacheCheckOnlyForEnabled) {
  auto autd = create_controller();
  autd.geometry()[0].set_enable(false);

  size_t cnt = 0;
  const auto g = ForCacheTest(&cnt).with_cache();
  autd.send(g);

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}
