#include <gtest/gtest.h>

#include <autd3/gain/holo/amplitude.hpp>
#include <numbers>

TEST(GainHolo, AmplitudeDB) {
  const auto amp = 121.5 * autd3::gain::holo::dB;
  ASSERT_EQ(amp.as_spl(), 121.5);
  ASSERT_EQ(amp.as_pascal(), 23.77004454874038);
}

TEST(GainHolo, AmplitudeSPL) {
  const auto amp = 23.77004454874038 * autd3::gain::holo::Pa;
  ASSERT_EQ(amp.as_spl(), 121.5);
  ASSERT_EQ(amp.as_pascal(), 23.77004454874038);
}
