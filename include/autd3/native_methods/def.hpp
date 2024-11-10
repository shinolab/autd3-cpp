#pragma once

#include <chrono>
#include <optional>

#include "autd3/def.hpp"

namespace autd3::native_methods {

struct FfiFutureResultStatus {
  uint8_t _data[24];
};
struct LocalFfiFutureResultStatus {
  uint8_t _data[24];
};
struct FfiFutureResultFPGAStateList {
  uint8_t _data[24];
};
struct FfiFutureResultFirmwareVersionList {
  uint8_t _data[24];
};
struct FfiFutureResultController {
  uint8_t _data[24];
};

struct Vector3 {
  float x;
  float y;
  float z;

  operator driver::Vector3() const { return {x, y, z}; }
};

struct Quaternion {
  float x;
  float y;
  float z;
  float w;

  operator driver::Quaternion() const { return {w, x, y, z}; }
};

struct Duration {
  template <typename Rep, typename Period>
  Duration(const std::chrono::duration<Rep, Period>& d) : _ns(std::chrono::duration_cast<std::chrono::nanoseconds>(d).count()) {}
  Duration(uint64_t ns) : _ns(ns) {}

 private:
  uint64_t _ns;
};

struct OptionDuration {
  template <typename Rep, typename Period>
  OptionDuration(const std::optional<std::chrono::duration<Rep, Period>>& d)
      : _has_value(d.has_value()), _ns(std::chrono::duration_cast<std::chrono::nanoseconds>(d.value_or(std::chrono::nanoseconds(0))).count()) {}

 private:
  bool _has_value;
  uint64_t _ns;
};

}  // namespace autd3::native_methods
