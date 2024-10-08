#pragma once

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

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
    _##PARAM_NAME = static_cast<PARAM_T>(value); /* LCOV_EXCL_LINE */                     \
  }                                                                                       \
  template <std::integral I>                                                              \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                        \
    _##PARAM_NAME = static_cast<PARAM_T>(value); /* LCOV_EXCL_LINE */                     \
    return std::move(*static_cast<T*>(this));    /* LCOV_EXCL_LINE */                     \
  }                                                                                       \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                               \
  PARAM_T _##PARAM_NAME;                                                                  \
                                                                                          \
 public:

#define AUTD3_DEF_PARAM_INTENSITY(T, PARAM_NAME)                                                               \
  template <std::integral I>                                                                                   \
  void with_##PARAM_NAME(I value)& {                                                                           \
    _##PARAM_NAME = autd3::driver::EmitIntensity(value); /* LCOV_EXCL_LINE */                                  \
  }                                                                                                            \
  template <std::integral I>                                                                                   \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                                             \
    _##PARAM_NAME = autd3::driver::EmitIntensity(value); /* LCOV_EXCL_LINE */                                  \
    return std::move(*static_cast<T*>(this));            /* LCOV_EXCL_LINE */                                  \
  }                                                                                                            \
  void with_##PARAM_NAME(autd3::driver::EmitIntensity value)& { _##PARAM_NAME = value; } /* LCOV_EXCL_LINE */  \
  [[nodiscard]] T&& with_##PARAM_NAME(autd3::driver::EmitIntensity value)&& {                                  \
    _##PARAM_NAME = value;                    /* LCOV_EXCL_LINE */                                             \
    return std::move(*static_cast<T*>(this)); /* LCOV_EXCL_LINE */                                             \
  }                                                                                                            \
  [[nodiscard]] autd3::driver::EmitIntensity PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                                                    \
  autd3::driver::EmitIntensity _##PARAM_NAME;                                                                  \
                                                                                                               \
 public:

#define AUTD3_DEF_PARAM_PHASE(T, PARAM_NAME)                                                           \
  template <std::integral I>                                                                           \
  void with_##PARAM_NAME(I value)& {                                                                   \
    _##PARAM_NAME = autd3::driver::Phase(value); /* LCOV_EXCL_LINE */                                  \
  }                                                                                                    \
  template <std::integral I>                                                                           \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                                     \
    _##PARAM_NAME = autd3::driver::Phase(value); /* LCOV_EXCL_LINE */                                  \
    return std::move(*static_cast<T*>(this));    /* LCOV_EXCL_LINE */                                  \
  }                                                                                                    \
  void with_##PARAM_NAME(autd3::driver::Phase value)& { _##PARAM_NAME = value; } /* LCOV_EXCL_LINE */  \
  [[nodiscard]] T&& with_##PARAM_NAME(autd3::driver::Phase value)&& {                                  \
    _##PARAM_NAME = value;                    /* LCOV_EXCL_LINE */                                     \
    return std::move(*static_cast<T*>(this)); /* LCOV_EXCL_LINE */                                     \
  }                                                                                                    \
  [[nodiscard]] autd3::driver::Phase PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                                            \
  autd3::driver::Phase _##PARAM_NAME;                                                                  \
                                                                                                       \
 public:

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

#ifdef AUTD3_ASYNC_API
inline coro::task<ResultI32> wait_future(const HandlePtr handle, FfiFutureResultI32 future) {
  co_return AUTDWaitResultI32(handle, std::move(future));
}

inline coro::task<ResultI32> wait_future(const HandlePtr handle, LocalFfiFutureResultI32 future) {
  co_return AUTDWaitLocalResultI32(handle, std::move(future));
}

inline coro::task<ResultFPGAStateList> wait_future(const HandlePtr handle, FfiFutureResultFPGAStateList future) {
  co_return AUTDWaitResultFPGAStateList(handle, std::move(future));
}

inline coro::task<ResultFirmwareVersionList> wait_future(const HandlePtr handle, FfiFutureResultFirmwareVersionList future) {
  co_return AUTDWaitResultFirmwareVersionList(handle, std::move(future));
}

inline coro::task<ResultController> wait_future(const HandlePtr handle, FfiFutureResultController future) {
  co_return AUTDWaitResultController(handle, std::move(future));
}
#endif

}  // namespace autd3::native_methods
