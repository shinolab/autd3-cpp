#pragma once

#include <unordered_map>

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class F>
concept group_f = requires(F f, const geometry::Device& dev) { typename std::invoke_result_t<F, const geometry::Device&>::value_type; };

template <group_f F>
struct Group final : Datagram {
  AUTD3_API explicit Group(
      F key_map, std::unordered_map<typename std::invoke_result_t<F, const geometry::Device&>::value_type, std::shared_ptr<Datagram>> data_map)
      : _keymap_f(std::move(key_map)), _data_map(std::move(data_map)) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    std::vector<native_methods::DatagramPtr> datagrams;
    std::vector<int32_t> keys;

    int k = 0;
    for (const auto& [key, v] : _data_map) {
      const auto ptr = v->ptr(geometry);
      datagrams.push_back(ptr);
      keys.push_back(k);
      _keymap[key] = k++;
    }

    const auto f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> int32_t {
      auto* self = static_cast<Group*>(const_cast<void*>(context));

      const geometry::Device dev(dev_idx, geometry_ptr);

      const auto key = self->_keymap_f(dev);
      return key.has_value() ? self->_keymap.at(key.value()) : -1;
    };

    return AUTDDatagramGroup(reinterpret_cast<const void*>(f_native), static_cast<const void*>(this), geometry.ptr(), keys.data(), datagrams.data(),
                             static_cast<uint16_t>(keys.size()));
  }

 private:
  F _keymap_f;
  std::unordered_map<typename std::invoke_result_t<F, const geometry::Device&>::value_type, std::shared_ptr<Datagram>> _data_map;
  mutable std::unordered_map<typename std::invoke_result_t<F, const geometry::Device&>::value_type, int32_t> _keymap = {};
};

}  // namespace autd3::driver