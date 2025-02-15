#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

template <class F>
concept gain_group_f = requires(F f, const driver::geometry::Device& dev, const driver::geometry::Transducer& tr) {
  typename std::invoke_result_t<std::invoke_result_t<F, const driver::geometry::Device&>, const driver::geometry::Transducer&>::value_type;
};

template <gain_group_f F>
class Group final : public driver::Gain, public driver::IntoDatagramTuple<Group<F>> {
 public:
  using key_type =
      typename std::invoke_result_t<std::invoke_result_t<F, const driver::geometry::Device&>, const driver::geometry::Transducer&>::value_type;

  template <driver::gain G>
  AUTD3_API explicit Group(F key_map, std::unordered_map<key_type, G> gain_map) : key_map(std::move(key_map)) {
    for (auto& [k, g] : gain_map) this->gain_map[k] = std::make_shared<std::remove_reference_t<G>>(std::forward<std::remove_reference_t<G>>(g));
  }
  AUTD3_API explicit Group(F key_map, std::unordered_map<key_type, std::shared_ptr<driver::Gain>> gain_map)
      : key_map(std::move(key_map)), gain_map(std::move(gain_map)) {}

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    std::unordered_map<key_type, int32_t> keymap;

    auto view = geometry.devices() | std::views::transform([](const driver::geometry::Device& dev) { return static_cast<uint16_t>(dev.idx()); });
    const std::vector<uint16_t> device_indices(view.begin(), view.end());

    auto map = native_methods::AUTDGainGroupCreateMap(device_indices.data(), static_cast<uint16_t>(device_indices.size()));
    int32_t k = 0;
    for (const auto& dev : geometry.devices()) {
      std::vector<int32_t> m;
      m.reserve(dev.num_transducers());
      std::for_each(dev.cbegin(), dev.cend(), [this, &dev, &m, &keymap, &k](const auto& tr) {
        if (auto key = this->key_map(dev)(tr); key.has_value()) {
          if (!keymap.contains(key.value())) {
            keymap[key.value()] = k++;
          }
          m.emplace_back(keymap[key.value()]);
        } else {
          m.emplace_back(-1);
        }
      });
      map = AUTDGainGroupMapSet(map, static_cast<uint16_t>(dev.idx()), m.data());
    }
    std::vector<int32_t> gain_keys;
    std::vector<native_methods::GainPtr> gain_ptrs;
    for (auto& kv : gain_map) {
      if (!keymap.contains(kv.first)) throw AUTDException("Unknown group key");
      gain_keys.emplace_back(keymap[kv.first]);
      gain_ptrs.emplace_back(kv.second->gain_ptr(geometry));
    }

    return AUTDGainGroup(map, gain_keys.data(), gain_ptrs.data(), static_cast<uint32_t>(gain_keys.size()));
  }

  F key_map;
  std::unordered_map<key_type, std::shared_ptr<driver::Gain>> gain_map;
};

}  // namespace autd3::gain
