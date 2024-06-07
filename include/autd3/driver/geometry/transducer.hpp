#pragma once

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Transducer {
 public:
  AUTD3_API Transducer(const uint8_t idx, const native_methods::DevicePtr ptr) : _ptr(AUTDTransducer(ptr, idx)), _idx(idx) {}

  AUTD3_API [[nodiscard]] Vector3 position() const noexcept {
    Vector3 v;
    AUTDTransducerPosition(_ptr, v.data());
    return v;
  }

  AUTD3_API [[nodiscard]] size_t idx() const noexcept { return _idx; }

  AUTD3_API [[nodiscard]] native_methods::TransducerPtr ptr() const { return _ptr; }

 private:
  native_methods::TransducerPtr _ptr;
  uint32_t _idx;
};

}  // namespace autd3::driver::geometry
