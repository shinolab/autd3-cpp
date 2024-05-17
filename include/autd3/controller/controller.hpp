#pragma once

#include <chrono>
#include <future>
#include <optional>
#include <vector>

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/firmware/fpga/fpga_state.hpp"
#include "autd3/driver/firmware_version.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {

class ControllerBuilder;

template <class F>
concept group_f =
    requires(F f, const driver::geometry::Device& dev) { typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type; };

template <typename L>
class Controller {
  friend class ControllerBuilder;

 public:
  Controller() = delete;
  Controller(const Controller& v) = delete;
  Controller& operator=(const Controller& obj) = delete;
  Controller(Controller&& obj) noexcept : _geometry(std::move(obj._geometry)), _ptr(obj._ptr), _link(std::move(obj._link)) { obj._ptr._0 = nullptr; }
  Controller& operator=(Controller&& obj) noexcept {
    if (this != &obj) {
      _geometry = std::move(obj._geometry);
      _ptr = obj._ptr;
      _link = std::move(obj._link);
      obj._ptr._0 = nullptr;
    }
    return *this;
  }

  ~Controller() noexcept {
    if (_ptr._0 != nullptr) {
      AUTDControllerDelete(_ptr);
      _ptr._0 = nullptr;
    }
  }

  AUTD3_API [[nodiscard]] const driver::geometry::Geometry& geometry() const { return _geometry; }
  AUTD3_API [[nodiscard]] driver::geometry::Geometry& geometry() { return _geometry; }

  AUTD3_API [[nodiscard]] L& link() { return _link; }
  AUTD3_API [[nodiscard]] const L& link() const { return _link; }

  AUTD3_API void close() const { validate(AUTDControllerClose(_ptr)); }

  AUTD3_API [[nodiscard]] std::vector<std::optional<driver::FPGAState>> fpga_info() {
    const size_t num_devices = geometry().num_devices();
    std::vector<int32_t> info(num_devices);
    validate(AUTDControllerFPGAState(_ptr, info.data()));
    std::vector<std::optional<driver::FPGAState>> ret;
    ret.reserve(num_devices);
    std::ranges::transform(info, std::back_inserter(ret), [](const int32_t i) -> std::optional<driver::FPGAState> {
      return i < 0 ? std::nullopt : std::optional(driver::FPGAState(static_cast<uint8_t>(i)));
    });
    return ret;
  }

  AUTD3_API [[nodiscard]] std::vector<driver::FirmwareVersion> firmware_version() {
    const auto handle = validate(AUTDControllerFirmwareVersionListPointer(_ptr));
    std::vector<driver::FirmwareVersion> ret;
    for (uint32_t i = 0; i < static_cast<uint32_t>(geometry().num_devices()); i++) {
      char info[256]{};
      AUTDControllerFirmwareVersionGet(handle, i, info);
      ret.emplace_back(std::string(info));
    }
    AUTDControllerFirmwareVersionListPointerDelete(handle);
    return ret;
  }

  template <driver::datagram D, typename Rep, typename Period>
  AUTD3_API void send(D&& data, const std::chrono::duration<Rep, Period> timeout) {
    send(std::forward<D>(data), std::optional(timeout));
  }

  template <driver::datagram D, typename Rep = uint64_t, typename Period = std::milli>
  AUTD3_API void send(D&& data, const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
    send(std::forward<D>(data), driver::NullDatagram(), timeout);
  }

  template <driver::datagram D1, driver::datagram D2, typename Rep, typename Period>
  AUTD3_API void send(D1&& data1, D2&& data2, const std::chrono::duration<Rep, Period> timeout) {
    send(std::forward<D1>(data1), std::forward<D2>(data2), std::optional(timeout));
  }

  template <driver::datagram D1, driver::datagram D2, typename Rep = uint64_t, typename Period = std::milli>
  AUTD3_API void send(D1&& data1, D2&& data2, const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
    const int64_t timeout_ns = timeout.has_value() ? std::chrono::duration_cast<std::chrono::nanoseconds>(timeout.value()).count() : -1;
    validate(AUTDControllerSend(_ptr, data1.ptr(_geometry), data2.ptr(_geometry), timeout_ns));
  }

  template <group_f F>
  class GroupGuard {
   public:
    using key_type = typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type;

    AUTD3_API explicit GroupGuard(F map, Controller& controller)
        : _controller(controller), _map(std::move(map)), _kv_map(native_methods::AUTDControllerGroupCreateKVMap()) {}

    template <driver::datagram D, typename Rep = uint64_t, typename Period = std::milli>
    AUTD3_API GroupGuard set(const key_type key, D&& data, const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
      if (_keymap.contains(key)) throw AUTDException("Key already exists");
      const int64_t timeout_ns = timeout.has_value() ? timeout.value().count() : -1;
      const auto ptr = data.ptr(_controller._geometry);
      _keymap[key] = _k++;
      native_methods::AUTDControllerGroupKVMapSet(_kv_map, _keymap[key], ptr, native_methods::DatagramPtr{nullptr}, timeout_ns);
      return std::move(*this);
    }

    template <driver::datagram D, typename Rep, typename Period>
    AUTD3_API GroupGuard set(const key_type key, D&& data, const std::chrono::duration<Rep, Period> timeout) {
      return set(key, std::forward<D>(data), std::optional(timeout));
    }

    template <driver::datagram D1, driver::datagram D2, typename Rep = uint64_t, typename Period = std::milli>
    AUTD3_API GroupGuard set(const key_type key, D1&& data1, D2&& data2,
                             const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
      if (_keymap.contains(key)) throw AUTDException("Key already exists");
      const int64_t timeout_ns = timeout.has_value() ? timeout.value().count() : -1;
      const auto ptr1 = data1.ptr(_controller._geometry);
      const auto ptr2 = data2.ptr(_controller._geometry);
      _keymap[key] = _k++;
      native_methods::AUTDControllerGroupKVMapSet(_kv_map, _keymap[key], ptr1, ptr2, timeout_ns);
      return std::move(*this);
    }

    template <driver::datagram D1, driver::datagram D2, typename Rep, typename Period>
    AUTD3_API GroupGuard set(const key_type key, D1&& data1, D2&& data2, const std::chrono::duration<Rep, Period> timeout) {
      return set(key, std::forward<D1>(data1), std::forward<D2>(data2), std::optional(timeout));
    }

    AUTD3_API void send() {
      std::vector<int32_t> map;
      map.reserve(_controller.geometry().num_devices());
      std::transform(_controller.geometry().cbegin(), _controller.geometry().cend(), std::back_inserter(map),
                     [this](const driver::geometry::Device& d) {
                       if (!d.enable()) return -1;
                       const auto k = _map(d);
                       return k.has_value() ? _keymap[k.value()] : -1;
                     });
      validate(AUTDControllerGroup(_controller._ptr, map.data(), _kv_map));
    }

#ifdef AUTD3_ASYNC_API
    AUTD3_API [[nodiscard]] coro::task<void> send_async() { co_return send(); }
#endif

   private:
    Controller& _controller;
    F _map;
    native_methods::GroupKVMapPtr _kv_map;
    std::unordered_map<key_type, int32_t> _keymap;
    int32_t _k{0};
  };

  template <group_f F>
  AUTD3_API GroupGuard<F> group(const F& map) {
    return GroupGuard<F>(map, *this);
  }

#ifdef AUTD3_ASYNC_API
  AUTD3_API [[nodiscard]] coro::task<void> close_async() const { co_return close(); }

  AUTD3_API [[nodiscard]] coro::task<std::vector<std::optional<driver::FPGAState>>> fpga_info_async() { co_return fpga_info(); }

  template <driver::datagram D, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] coro::task<void> send_async(D&& data, const std::chrono::duration<Rep, Period> timeout) {
    co_await send_async(std::forward<D>(data), std::optional(timeout));
    co_return;
  }

  template <driver::datagram D, typename Rep = uint64_t, typename Period = std::milli>
  AUTD3_API [[nodiscard]] coro::task<void> send_async(D&& data, const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
    co_await send_async(std::forward<D>(data), driver::NullDatagram(), timeout);
    co_return;
  }

  template <driver::datagram D1, driver::datagram D2, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] coro::task<void> send_async(D1&& data1, D2&& data2, const std::chrono::duration<Rep, Period> timeout) {
    co_await send_async(std::forward<D1>(data1), std::forward<D2>(data2), std::optional(timeout));
    co_return;
  }

  template <driver::datagram D1, driver::datagram D2, typename Rep = uint64_t, typename Period = std::milli>
  AUTD3_API [[nodiscard]] coro::task<void> send_async(D1&& data1, D2&& data2,
                                                      const std::optional<std::chrono::duration<Rep, Period>> timeout = std::nullopt) {
    const int64_t timeout_ns = timeout.has_value() ? std::chrono::duration_cast<std::chrono::nanoseconds>(timeout.value()).count() : -1;
    validate(AUTDControllerSend(_ptr, data1.ptr(_geometry), data2.ptr(_geometry), timeout_ns));
    co_return;
  }

  AUTD3_API [[nodiscard]] coro::task<std::vector<driver::FirmwareVersion>> firmware_version_async() { co_return firmware_version(); }
#endif

 private:
  AUTD3_API Controller(driver::geometry::Geometry geometry, const native_methods::ControllerPtr ptr, L link)
      : _geometry(std::move(geometry)), _ptr(ptr), _link(std::move(link)) {}

  driver::geometry::Geometry _geometry;
  native_methods::ControllerPtr _ptr;
  L _link;
};

}  // namespace autd3::controller
