#pragma once

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Transducer {
 public:
  Transducer(const uint32_t idx, const native_methods::DevicePtr ptr) : _ptr(AUTDTransducer(ptr, idx)), _idx(idx) {}

  [[nodiscard]] Vector3 position() const noexcept {
    Vector3 v;
    AUTDTransducerPosition(_ptr, v.data());
    return v;
  }

  [[nodiscard]] Quaternion rotation() const noexcept {
    double v[4];
    AUTDTransducerRotation(_ptr, v);
    return {v[0], v[1], v[2], v[3]};
  }

  [[nodiscard]] size_t idx() const noexcept { return _idx; }

  [[nodiscard]] Vector3 x_direction() const {
    Vector3 v;
    AUTDTransducerDirectionX(_ptr, v.data());
    return v;
  }

  [[nodiscard]] Vector3 y_direction() const {
    Vector3 v;
    AUTDTransducerDirectionY(_ptr, v.data());
    return v;
  }

  [[nodiscard]] Vector3 z_direction() const {
    Vector3 v;
    AUTDTransducerDirectionZ(_ptr, v.data());
    return v;
  }

  [[nodiscard]] native_methods::TransducerPtr ptr() const { return _ptr; }

  // LCOV_EXCL_START
 private:
  native_methods::TransducerPtr _ptr;
  uint32_t _idx;
  // LCOV_EXCL_STOP
};

}  // namespace autd3::driver::geometry
