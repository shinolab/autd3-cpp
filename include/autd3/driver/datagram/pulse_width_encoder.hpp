#pragma once

#include <optional>
#include <shared_mutex>
#include <unordered_map>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/pulse_width.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct PulseWidthEncoder final : Datagram, IntoDatagramTuple<PulseWidthEncoder> {
  AUTD3_API PulseWidthEncoder() : _f(std::nullopt) {}
  AUTD3_API explicit PulseWidthEncoder(std::function<std::function<PulseWidth(Intensity)>(const geometry::Device&)> f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t i) -> uint16_t {
      auto* self = static_cast<PulseWidthEncoder*>(const_cast<void*>(context));
      bool contains;
      {
        std::shared_lock lock(self->_mtx);
        contains = self->_cache.contains(dev_idx);
      }
      const geometry::Device dev(dev_idx, geometry_ptr);
      if (contains) return self->_cache[dev_idx](Intensity(i)).pulse_width();
      auto h = self->_f.value()(dev);  // LCOV_EXCL_LINE
      const auto res = h(Intensity(i)).pulse_width();
      {
        std::lock_guard lock(self->_mtx);
        self->_cache[dev_idx] = std::move(h);
      }
      return res;
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return _f.has_value() ? AUTDDatagramPulseWidthEncoder512(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr())
                          : native_methods::AUTDDatagramPulseWidthEncoder512Default();
  }

 private:
  using native_f = uint16_t (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t);

  std::optional<std::function<std::function<PulseWidth(Intensity)>(const geometry::Device&)>> _f;
  native_f _f_native = nullptr;
  std::shared_mutex _mtx;
  std::unordered_map<uint16_t, std::function<PulseWidth(Intensity)>> _cache;
};

}  // namespace autd3::driver
