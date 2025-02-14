#pragma once

#include "autd3/gain/holo/backend.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class NalgebraBackend final : public Backend {
 public:
  NalgebraBackend() : Backend(native_methods::AUTDNalgebraBackendSphere()) {}
  AUTD3_API ~NalgebraBackend() override {
    if (_ptr._0 != nullptr) {
      AUTDDeleteNalgebraBackendSphere(_ptr);
      _ptr._0 = nullptr;
    }
  }
  NalgebraBackend(const NalgebraBackend& v) noexcept = delete;
  NalgebraBackend& operator=(const NalgebraBackend& obj) = delete;
  NalgebraBackend(NalgebraBackend&& obj) = default;             // LCOV_EXCL_LINE
  NalgebraBackend& operator=(NalgebraBackend&& obj) = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gs(const float* foci, const float* amps, const uint32_t size,
                                                     const native_methods::GSOption option) const override {
    return AUTDGainHoloGSSphere(_ptr, reinterpret_cast<const native_methods::Point3*>(foci), amps, size, option);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr gspat(const float* foci, const float* amps, const uint32_t size,
                                                        const native_methods::GSPATOption option) const override {
    return AUTDGainHoloGSPATSphere(_ptr, reinterpret_cast<const native_methods::Point3*>(foci), amps, size, option);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr naive(const float* foci, const float* amps, const uint32_t size,
                                                        const native_methods::NaiveOption option) const override {
    return AUTDGainHoloNaiveSphere(_ptr, reinterpret_cast<const native_methods::Point3*>(foci), amps, size, option);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr lm(const float* foci, const float* amps, const uint32_t size,
                                                     const native_methods::LMOption option) const override {
    return AUTDGainHoloLMSphere(_ptr, reinterpret_cast<const native_methods::Point3*>(foci), amps, size, option);
  }
};
}  // namespace autd3::gain::holo
