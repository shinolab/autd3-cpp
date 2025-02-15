#include <gtest/gtest.h>

#include <autd3/driver/defined/freq.hpp>
#include <sstream>

TEST(DriverDefined, Freq) {
  std::stringstream ss;
  ss << 1u * autd3::driver::Hz;
  ASSERT_EQ(ss.str(), "1 Hz");
}
