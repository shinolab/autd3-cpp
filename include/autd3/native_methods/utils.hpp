

#pragma once

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

#define AUTD3_DEF_PROP(PARAM_T, PARAM_NAME)                                               \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
                                                                                          \
 protected:                                                                               \
  PARAM_T _##PARAM_NAME;                                                                  \
                                                                                          \
 public:

#define AUTD3_DEF_PARAM(T, PARAM_T, PARAM_NAME)                                                     \
  void with_##PARAM_NAME(PARAM_T value)& { _##PARAM_NAME = std::move(value); } /* LCOV_EXCL_LINE */ \
  [[nodiscard]] T&& with_##PARAM_NAME(PARAM_T value)&& {                       /* LCOV_EXCL_LINE */ \
    _##PARAM_NAME = std::move(value);                                          /* LCOV_EXCL_LINE */ \
    return std::move(*static_cast<T*>(this));                                  /* LCOV_EXCL_LINE */ \
  }                                                                                                 \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */           \
                                                                                                    \
 protected:                                                                                         \
  PARAM_T _##PARAM_NAME;                                                                            \
                                                                                                    \
 public:

#define AUTD3_DEF_PARAM_INT(T, PARAM_T, PARAM_NAME)                                       \
  template <std::integral I>                                                              \
  void with_##PARAM_NAME(I value)& {                                                      \
    _##PARAM_NAME = value; /* LCOV_EXCL_LINE */                                           \
  }                                                                                       \
  template <std::integral I>                                                              \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                        \
    _##PARAM_NAME = value;                    /* LCOV_EXCL_LINE */                        \
    return std::move(*static_cast<T*>(this)); /* LCOV_EXCL_LINE */                        \
  }                                                                                       \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                               \
  PARAM_T _##PARAM_NAME;                                                                  \
                                                                                          \
 public:

namespace autd3::native_methods {

template <class T>
concept result_ptr = requires(T& x) { x.result._0; };

template <result_ptr T>
constexpr auto validate(T res) {
  if (res.result._0 == nullptr) {
    const std::string err(res.err_len, ' ');
    AUTDGetErr(res.err, const_cast<char*>(err.c_str()));
    throw AUTDException(err);
  }
  return res.result;
}

template <typename T = int32_t>
constexpr T validate(ResultI32 res) {
  const auto [result, err_len, err] = res;
  if (result == AUTD3_ERR) {
    const std::string err_str(err_len, ' ');
    AUTDGetErr(err, const_cast<char*>(err_str.c_str()));
    throw AUTDException(err_str);
  }
  return static_cast<T>(result);
}

}  // namespace autd3::native_methods
