#pragma once

#include <memory>

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
struct Cache final : driver::Modulation, driver::IntoDatagramTuple<Cache<M>> {
  AUTD3_API explicit Cache(M m) : m(std::move(m)), _ptr(nullptr) {}
  Cache(const Cache& v) = default;  // LCOV_EXCL_LINE
  Cache& operator=(const Cache& obj) = delete;
  Cache(Cache&& obj) noexcept = default;  // LCOV_EXCL_LINE
  Cache& operator=(Cache&& obj) noexcept = delete;
  ~Cache() noexcept override = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    if (!_ptr) {
      _ptr = std::shared_ptr<void>(const_cast<void*>(native_methods::AUTDModulationCache(m.modulation_ptr())._0), [](const void* ptr) {
        if (!ptr) return;
        AUTDModulationCacheFree(native_methods::ModulationCachePtr{ptr});
      });
    }
    return AUTDModulationCacheClone(native_methods::ModulationCachePtr{_ptr.get()});
  }

  M m;

 private:
  mutable std::shared_ptr<void> _ptr;
};

}  // namespace autd3::modulation
