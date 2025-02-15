#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class Backend {
 public:
  Backend() noexcept : _ptr(native_methods::BackendPtr{nullptr}) {}
  explicit Backend(const native_methods::BackendPtr ptr) noexcept : _ptr(ptr) {}
  Backend(const Backend& obj) = default;             // LCOV_EXCL_LINE
  Backend& operator=(const Backend& obj) = default;  // LCOV_EXCL_LINE
  Backend(Backend&& obj) = default;                  // LCOV_EXCL_LINE
  Backend& operator=(Backend&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~Backend() = default;                      // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::BackendPtr ptr() const { return _ptr; }

  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gs(const float* foci, const float* amps, uint32_t size,
                                                             native_methods::GSOption option) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gspat(const float* foci, const float* amps, uint32_t size,
                                                                native_methods::GSPATOption option) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr naive(const float* foci, const float* amps, uint32_t size,
                                                                native_methods::NaiveOption option) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr lm(const float* foci, const float* amps, uint32_t size,
                                                             native_methods::LMOption option) const = 0;

 protected:
  native_methods::BackendPtr _ptr;
};

template <class B>
concept backend = requires(B b) {
  { b.ptr() } -> std::same_as<native_methods::BackendPtr>;
};

}  // namespace autd3::gain::holo
