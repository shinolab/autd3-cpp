#pragma once

#include <memory>

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
class Cache final : public driver::ModulationBase<Cache<M>> {
 public:
  explicit Cache(M m) : _m(std::move(m)), _cache(std::make_shared<std::vector<uint8_t>>()) {}
  Cache(const Cache& v) = default;
  Cache& operator=(const Cache& obj) = delete;
  Cache(Cache&& obj) noexcept = default;
  Cache& operator=(Cache&& obj) noexcept = delete;
  ~Cache() noexcept override = default;

  const std::vector<uint8_t>& calc(const driver::geometry::Geometry& geometry) const { return init(geometry); }

  [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry& geometry) const override {
    const auto& buf = calc(geometry);
    return native_methods::AUTDModulationRaw(_sampling_config.value(), _m.loop_behavior(), buf.data(), static_cast<uint32_t>(buf.size()));
  }

  [[nodiscard]] const std::vector<uint8_t>& buffer() const { return *_cache; }

  [[nodiscard]] std::vector<uint8_t>::const_iterator cbegin() const noexcept { return _cache->cbegin(); }
  [[nodiscard]] std::vector<uint8_t>::const_iterator cend() const noexcept { return _cache->cend(); }
  [[nodiscard]] std::vector<uint8_t>::const_iterator begin() const noexcept { return _cache->begin(); }
  [[nodiscard]] std::vector<uint8_t>::const_iterator end() const noexcept { return _cache->end(); }
  [[nodiscard]] const uint8_t& operator[](const size_t i) const { return _cache->at(i); }

 private:
  const std::vector<uint8_t>& init(const driver::geometry::Geometry& geometry) const {
    if (_cache->size() == 0) {
      const auto res = native_methods::AUTDModulationCalc(_m.modulation_ptr(geometry), geometry.ptr());
      const auto ptr = validate(res);
      _cache->resize(native_methods::AUTDModulationCalcGetSize(ptr), uint8_t(0));
      _sampling_config = res.config;
      native_methods::AUTDModulationCalcGetResult(ptr, reinterpret_cast<uint8_t*>(_cache->data()));
      native_methods::AUTDModulationCalcFreeResult(ptr);
    }
    return *_cache;
  }

  M _m;
  mutable std::shared_ptr<std::vector<uint8_t>> _cache;
  mutable std::optional<native_methods::SamplingConfigWrap> _sampling_config;
};

}  // namespace autd3::modulation

namespace autd3::driver {

template <class M>
class IntoModulationCache {
 public:
  [[nodiscard]] modulation::Cache<M> with_cache() & { return modulation::Cache(*static_cast<M*>(this)); }
  [[nodiscard]] modulation::Cache<M> with_cache() && { return modulation::Cache(std::move(*static_cast<M*>(this))); }
};

}  // namespace autd3::driver
