#pragma once

#include <optional>

#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class PulseWidthEncoder final : public IntoDatagramWithTimeout<PulseWidthEncoder>, public IntoDatagramWithParallelThreshold<PulseWidthEncoder> {
  using native_f = uint16_t (*)(const void*, native_methods::GeometryPtr, uint16_t, uint16_t);

 public:
  AUTD3_API PulseWidthEncoder() : _f(std::nullopt) {}
  AUTD3_API explicit PulseWidthEncoder(std::function<std::function<uint16_t(size_t)>(const geometry::Device&)> f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint16_t i) -> uint16_t {
      const geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      return static_cast<const PulseWidthEncoder*>(context)->_f.value()(dev)(i);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return _f.has_value()
               ? AUTDDatagramPulseWidthEncoder(const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                               native_methods::ContextPtr{const_cast<void*>(static_cast<const void*>(this))}, geometry.ptr())
               : native_methods::AUTDDatagramPulseWidthEncoderDefault();
  }

 private:
  std::optional<std::function<std::function<uint16_t(size_t)>(const geometry::Device&)>> _f;
  native_f _f_native = nullptr;
};

}  // namespace autd3::driver
