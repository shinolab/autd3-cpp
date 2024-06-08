#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class F>
concept _force_fan_f = requires(F f, const geometry::Device& d) {
  { f(d) } -> std::same_as<bool>;
};

template <_force_fan_f F>
class ForceFan final : public IntoDatagramTuple<ForceFan<F>>,
                       public IntoDatagramWithTimeout<ForceFan<F>>,
                       public IntoDatagramWithParallelThreshold<ForceFan<F>> {
  using native_f = bool (*)(const void*, native_methods::GeometryPtr, uint16_t);

 public:
  AUTD3_API explicit ForceFan(F f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> bool {
      const geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      return static_cast<const ForceFan*>(context)->_f(dev);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDDatagramForceFan(const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                native_methods::ContextPtr{const_cast<void*>(static_cast<const void*>(this))}, geometry.ptr());
  }

 private:
  F _f;
  native_f _f_native;
};

}  // namespace autd3::driver
