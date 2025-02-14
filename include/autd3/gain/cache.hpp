#pragma once

#include <algorithm>
#include <memory>

#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

template <class G>
class Cache final : public driver::Gain, public driver::IntoDatagramTuple<Cache<G>> {
 public:
  AUTD3_API explicit Cache(G g) : gain(std::move(g)), _ptr(nullptr) {}

  Cache() = delete;
  Cache(const Cache& obj) = default;             // LCOV_EXCL_LINE
  Cache& operator=(const Cache& obj) = default;  // LCOV_EXCL_LINE
  Cache(Cache&& obj) = default;                  // LCOV_EXCL_LINE
  Cache& operator=(Cache&& obj) = default;       // LCOV_EXCL_LINE
  ~Cache() override = default;                   // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    if (!_ptr) {
      _ptr = std::shared_ptr<void>(const_cast<void*>(native_methods::AUTDGainCache(gain.gain_ptr(geometry))._0), [](const void* ptr) {
        if (!ptr) return;
        AUTDGainCacheFree(native_methods::GainCachePtr{ptr});
      });
    }
    return AUTDGainCacheClone(native_methods::GainCachePtr{_ptr.get()});
  }

  G gain;

 private:
  mutable std::shared_ptr<void> _ptr;
};
}  // namespace autd3::gain
