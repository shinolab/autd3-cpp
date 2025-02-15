#pragma once

#include <chrono>
#include <optional>

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::native_methods {

template <typename T>
constexpr auto validate(T res) {
  if (res.err_len != 0) {
    const std::string err(res.err_len, ' ');
    AUTDGetErr(res.err, const_cast<char*>(err.c_str()));
    throw AUTDException(err);
  }
  return res.result;
}

template <typename T = AUTDStatus>
constexpr T validate(ResultStatus res) {
  const auto [result, err_len, err] = res;
  if (result == AUTDStatus::AUTDErr) {
    const std::string err_str(err_len, ' ');
    AUTDGetErr(err, const_cast<char*>(err_str.c_str()));
    throw AUTDException(err_str);
  }
  return static_cast<T>(result);
}

template <typename Rep, typename Period>
Duration to_duration(const std::chrono::duration<Rep, Period>& d) {
  return Duration{static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(d).count())};
}

inline std::chrono::nanoseconds from_duration(const Duration& d) { return std::chrono::nanoseconds(d.nanos); }

template <typename Rep, typename Period>
OptionDuration to_option_duration(const std::optional<std::chrono::duration<Rep, Period>>& d) {
  return OptionDuration{d.has_value(), to_duration(d.value_or(std::chrono::nanoseconds(0)))};
}

inline std::optional<std::chrono::nanoseconds> from_option_duration(const OptionDuration& d) {
  return d.has_value ? std::make_optional(from_duration(d.value)) : std::nullopt;
}

}  // namespace autd3::native_methods
