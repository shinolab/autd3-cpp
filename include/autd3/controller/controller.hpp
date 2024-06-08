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

  AUTD3_API [[nodiscard]] uint16_t last_parallel_threshold() { return native_methods::AUTDControllerLastParallelThreshold(_ptr); }

  AUTD3_API [[nodiscard]] L& link() { return _link; }
  AUTD3_API [[nodiscard]] const L& link() const { return _link; }

  AUTD3_API void close() const { validate(AUTDControllerClose(_ptr)); }

  AUTD3_API [[nodiscard]] std::vector<std::optional<driver::FPGAState>> fpga_state() {
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

  template <driver::datagram D>
  AUTD3_API void send(const D& d) {
    validate(AUTDControllerSend(_ptr, d.ptr(_geometry)));
  }

  template <group_f F>
  class GroupGuard {
   public:
    using key_type = typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type;
    using native_f = int32_t (*)(const void*, native_methods::GeometryPtr, uint16_t);

    AUTD3_API explicit GroupGuard(F map, Controller& controller) : _controller(controller), _map(std::move(map)) {
      _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> int32_t {
        const driver::geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
        const auto* self = static_cast<const GroupGuard*>(context);
        const auto key = self->_map(dev);
        const auto& keymap = self->_keymap;
        return key.has_value() ? keymap.at(key.value()) : -1;
      };
    }

    template <driver::datagram D>
    AUTD3_API GroupGuard set(const key_type key, const D& data) {
      if (_keymap.contains(key)) throw AUTDException("Key already exists");
      const auto ptr = data.ptr(_controller._geometry);
      _datagrams.push_back(ptr);
      _keys.push_back(_k);
      _keymap[key] = _k++;
      return std::move(*this);
    }

    AUTD3_API void send() const {
      validate(AUTDControllerGroup(_controller._ptr, const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                   native_methods::ContextPtr{const_cast<void*>(static_cast<const void*>(this))}, _controller._geometry.ptr(),
                                   _keys.data(), _datagrams.data(), static_cast<uint16_t>(_keys.size())));
    }

#ifdef AUTD3_ASYNC_API
    AUTD3_API [[nodiscard]] coro::task<void> send_async() { co_return send(); }
#endif

   private:
    Controller& _controller;
    F _map;
    native_f _f_native;
    std::vector<int32_t> _keys;
    std::vector<native_methods::DatagramPtr> _datagrams;
    std::unordered_map<key_type, int32_t> _keymap;
    int32_t _k{0};
  };

  template <group_f F>
  AUTD3_API GroupGuard<F> group(const F& map) {
    return GroupGuard<F>(map, *this);
  }

#ifdef AUTD3_ASYNC_API
  AUTD3_API [[nodiscard]] coro::task<void> close_async() const { co_return close(); }

  AUTD3_API
  [[nodiscard]] coro::task<std::vector<std::optional<driver::FPGAState>>> fpga_info_async() { co_return fpga_state(); }

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
