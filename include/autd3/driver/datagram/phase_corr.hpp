#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct PhaseCorrection final : Datagram, IntoDatagramTuple<PhaseCorrection> {
  AUTD3_API explicit PhaseCorrection(std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t tr_idx) -> uint8_t {
      auto* self = static_cast<PhaseCorrection*>(const_cast<void*>(context));
      bool contains;
      {
        std::shared_lock lock(self->_mtx);
        contains = self->_cache.contains(dev_idx);
      }
      const geometry::Transducer tr(tr_idx, dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      const geometry::Device dev(dev_idx, geometry_ptr);
      if (contains) return self->_cache[dev_idx](tr).value();
      auto h = self->f(dev);
      const auto res = h(tr);
      {
        std::lock_guard lock(self->_mtx);
        self->_cache[dev_idx] = std::move(h);
      }
      return res.value();
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDDatagramPhaseCorr(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> f;

 private:
  using native_f = uint8_t (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t);

  native_f _f_native = nullptr;
  std::shared_mutex _mtx;
  std::unordered_map<uint16_t, std::function<Phase(const geometry::Transducer&)>> _cache;
};

}  // namespace autd3::driver
