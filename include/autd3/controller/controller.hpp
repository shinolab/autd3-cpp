#pragma once

#include <future>
#include <optional>
#include <variant>
#include <vector>

#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/firmware/fpga/fpga_state.hpp"
#include "autd3/driver/firmware_version.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/link.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"
#include "sleeper.hpp"

namespace autd3::controller {

template <class F>
concept group_f =
    requires(F f, const driver::geometry::Device& dev) { typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type; };

struct SenderOption {
  std::chrono::nanoseconds send_interval = std::chrono::milliseconds(1);
  std::chrono::nanoseconds receive_interval = std::chrono::milliseconds(1);
  std::optional<std::chrono::nanoseconds> timeout = std::nullopt;
  native_methods::ParallelMode parallel = native_methods::ParallelMode::Auto;
  std::variant<StdSleeper, SpinSleeper, WaitableSleeper> sleeper = SpinSleeper();

  operator native_methods::SenderOption() const {
    return native_methods::SenderOption{.send_interval = native_methods::to_duration(send_interval),
                                        .receive_interval = native_methods::to_duration(receive_interval),
                                        .timeout = native_methods::to_option_duration(timeout),
                                        .parallel = parallel,
                                        .sleeper = std::visit([](const auto& s) { return native_methods::SleeperWrap(s); }, sleeper)};
  }
};

class Controller;

class Sender {
 public:
  friend Controller;

  Sender() = delete;
  Sender(const Sender& v) = default;
  Sender& operator=(const Sender& obj) = delete;
  Sender(Sender&& obj) = default;
  Sender& operator=(Sender&& obj) = delete;

  template <driver::datagram D>
  AUTD3_API void send(const D& d) {
    native_methods::validate(native_methods::AUTDSenderSend(_ptr, d.ptr(_geometry)));
  }

  template <group_f F>
  AUTD3_API void group_send(
      F key_map,
      std::unordered_map<typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type, std::shared_ptr<driver::Datagram>> data_map) {
    std::unordered_map<typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type, int32_t> keymap;
    std::vector<native_methods::DatagramPtr> datagrams;
    std::vector<int32_t> keys;

    int k = 0;
    for (const auto& [key, v] : data_map) {
      const auto ptr = v->ptr(_geometry);
      datagrams.push_back(ptr);
      keys.push_back(k);
      keymap[key] = k++;
    }

    const auto context = std::make_pair(std::move(key_map), std::move(keymap));

    const auto f_native = +[](const void* context_ptr, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> int32_t {
      const driver::geometry::Device dev(dev_idx, geometry_ptr);
      const auto* c = static_cast<
          const std::pair<F, std::unordered_map<typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type, int32_t>>*>(
          context_ptr);
      const auto key = c->first(dev);
      const auto& keymap_ = c->second;
      return key.has_value() ? keymap_.at(key.value()) : -1;
    };

    validate(AUTDControllerGroup(_ptr, reinterpret_cast<const void*>(f_native), static_cast<const void*>(&context), _geometry._geometry_ptr,
                                 keys.data(), datagrams.data(), static_cast<uint16_t>(keys.size())));
  }

 private:
  explicit Sender(native_methods::SenderPtr ptr, const driver::geometry::Geometry& geometry) : _ptr(ptr), _geometry(geometry) {}

  native_methods::SenderPtr _ptr;
  const driver::geometry::Geometry& _geometry;
};

class Controller final : public driver::geometry::Geometry {
 public:
  Controller() = delete;
  Controller(const Controller& v) = delete;
  Controller& operator=(const Controller& obj) = delete;
  Controller(Controller&& obj) noexcept : Geometry(obj._geometry_ptr, std::move(obj._devices)), _ptr(obj._ptr) {
    obj._geometry_ptr._0 = nullptr;
    obj._ptr._0 = nullptr;
  }
  Controller& operator=(Controller&& obj) noexcept {
    if (this != &obj) {
      this->_geometry_ptr = obj._geometry_ptr;
      this->_devices = std::move(obj._devices);
      _ptr = obj._ptr;
      obj._geometry_ptr._0 = nullptr;
      obj._ptr._0 = nullptr;
    }
    return *this;
  }

  ~Controller() noexcept override {
    try {
      close();
    } catch (...) {  // LCOV_EXCL_LINE
    }  // LCOV_EXCL_LINE
  }

  template <driver::link L>
  AUTD3_API [[nodiscard]] static Controller open(std::vector<driver::AUTD3> devices, L link) {
    return Controller::open_with_option(std::move(devices), link, SenderOption());
  }

  template <driver::link L>
  AUTD3_API [[nodiscard]] static Controller open_with_option(const std::vector<driver::AUTD3>& devices, L link, const SenderOption option) {
    std::vector<native_methods::Point3> pos;
    pos.reserve(devices.size());
    std::ranges::transform(devices, std::back_inserter(pos), [&](const auto& d) { return native_methods::Point3{d.pos.x(), d.pos.y(), d.pos.z()}; });

    std::vector<native_methods::Quaternion> rot;
    rot.reserve(devices.size());
    std::ranges::transform(devices, std::back_inserter(rot),
                           [&](const auto& d) { return native_methods::Quaternion{d.rot.x(), d.rot.y(), d.rot.z(), d.rot.w()}; });

    const auto ptr =
        validate(native_methods::AUTDControllerOpen(pos.data(), rot.data(), static_cast<uint16_t>(devices.size()), link.resolve(), option));
    auto geometry = AUTDGeometry(ptr);
    return Controller(geometry, ptr);
  }

  AUTD3_API [[nodiscard]] const Geometry& geometry() const { return static_cast<const Geometry&>(*this); }
  AUTD3_API [[nodiscard]] Geometry& geometry() { return static_cast<Geometry&>(*this); }

  template <driver::link L>
  AUTD3_API [[nodiscard]] L link() {
    return L{AUTDLinkGet(_ptr)};
  }

  AUTD3_API void close() {
    if (_ptr._0 == nullptr) return;
    const auto res = AUTDControllerClose(_ptr);
    _ptr._0 = nullptr;
    validate(res);
  }

  AUTD3_API [[nodiscard]] std::vector<std::optional<driver::FPGAState>> fpga_state() {
    const size_t num_devices = geometry().num_devices();
    const auto p = validate(AUTDControllerFPGAState(_ptr));
    std::vector<std::optional<driver::FPGAState>> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      const auto state = AUTDControllerFPGAStateGet(p, i);
      ret.emplace_back(state < 0 ? std::nullopt : std::optional(driver::FPGAState(static_cast<uint8_t>(state))));
    }
    AUTDControllerFPGAStateDelete(p);
    return ret;
  }  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] std::vector<driver::FirmwareVersion> firmware_version() {
    const size_t num_devices = geometry().num_devices();
    const auto handle = validate(AUTDControllerFirmwareVersionListPointer(_ptr));
    std::vector<driver::FirmwareVersion> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      char info[256]{};
      AUTDControllerFirmwareVersionGet(handle, i, info);
      ret.emplace_back(driver::FirmwareVersion{std::string(info)});
    }
    AUTDControllerFirmwareVersionListPointerDelete(handle);
    return ret;
  }  // LCOV_EXCL_LINE

  AUTD3_API Sender sender(const SenderOption option) const { return Sender(AUTDSender(_ptr, SenderOption()), geometry()); }

  template <driver::datagram D>
  AUTD3_API void send(const D& d) {
    sender(SenderOption()).send(d);
  }

  template <group_f F>
  AUTD3_API void group_send(
      F key_map,
      std::unordered_map<typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type, std::shared_ptr<driver::Datagram>> data_map) {
    sender(SenderOption()).group_send(std::move(key_map), std::move(data_map));
  }

 private:
  AUTD3_API Controller(const native_methods::GeometryPtr geometry, const native_methods::ControllerPtr ptr) : Geometry(geometry), _ptr(ptr) {}

  native_methods::ControllerPtr _ptr;
};

}  // namespace autd3::controller
