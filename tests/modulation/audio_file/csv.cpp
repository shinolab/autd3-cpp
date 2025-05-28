#include <gtest/gtest.h>

#include <autd3/modulation/audio_file/csv.hpp>

#include "utils.hpp"

TEST(Modulation, Csv) {
  auto autd = create_controller();

  const std::filesystem::path path = std::filesystem::path(AUTD3_RESOURCE_PATH).append("sin150.csv");
  autd.send(autd3::modulation::audio_file::Csv(path, 4000.0f * autd3::driver::Hz, autd3::modulation::audio_file::CsvOption()));

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{157, 185, 210, 231, 245, 253, 255, 249, 236, 218, 194, 167, 138, 108, 79,  53,  31,  14,  4,   0,
                                    4,   14,  31,  53,  79,  108, 138, 167, 194, 218, 236, 249, 255, 253, 245, 231, 210, 185, 157, 128,
                                    98,  70,  45,  24,  10,  2,   0,   6,   19,  37,  61,  88,  117, 147, 176, 202, 224, 241, 251, 255,
                                    251, 241, 224, 202, 176, 147, 117, 88,  61,  37,  19,  6,   0,   2,   10,  24,  45,  70,  98,  128};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().modulation_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
  }
}
