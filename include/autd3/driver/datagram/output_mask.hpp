#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct OutputMask final : Datagram, IntoDatagramTuple<OutputMask> {
  AUTD3_API explicit OutputMask(std::function<std::function<bool(const geometry::Transducer&)>(const geometry::Device&)> f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t tr_idx) -> bool {
      auto* self = static_cast<OutputMask*>(const_cast<void*>(context));
      bool contains;
      {
        std::shared_lock lock(self->_mtx);
        contains = self->_cache.contains(dev_idx);
      }
      const geometry::Transducer tr(tr_idx, dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      const geometry::Device dev(dev_idx, geometry_ptr);
      if (contains) return self->_cache[dev_idx](tr);
      auto h = self->f(dev);
      const auto res = h(tr);
      {
        std::lock_guard lock(self->_mtx);
        self->_cache[dev_idx] = std::move(h);
      }
      return res;
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDDatagramOutputMask(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  std::function<std::function<bool(const geometry::Transducer&)>(const geometry::Device&)> f;

 private:
  using native_f = bool (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t);

  native_f _f_native = nullptr;
  std::shared_mutex _mtx;
  std::unordered_map<uint16_t, std::function<bool(const geometry::Transducer&)>> _cache;
};

}  // namespace autd3::driver
