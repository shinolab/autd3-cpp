#pragma once

#include <future>
#include <optional>
#include <vector>

#include "autd3/controller/environment.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/firmware/fpga/fpga_state.hpp"
#include "autd3/driver/firmware_version.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/link.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {

struct SenderOption {
  std::optional<std::chrono::nanoseconds> send_interval = std::chrono::milliseconds(1);
  std::optional<std::chrono::nanoseconds> receive_interval = std::chrono::milliseconds(1);
  std::optional<std::chrono::nanoseconds> timeout = std::nullopt;

  operator native_methods::SenderOption() const {
    return native_methods::SenderOption{
        .send_interval = native_methods::to_option_duration(send_interval),
        .receive_interval = native_methods::to_option_duration(receive_interval),
        .timeout = native_methods::to_option_duration(timeout),
    };
  }

  auto operator<=>(const SenderOption&) const = default;
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

 private:
  explicit Sender(const native_methods::SenderPtr ptr, const driver::geometry::Geometry& geometry) : _ptr(ptr), _geometry(geometry) {}

  native_methods::SenderPtr _ptr;
  const driver::geometry::Geometry& _geometry;
};

class Controller final : public driver::geometry::Geometry {
 public:
  Controller() = delete;
  Controller(const Controller& v) = delete;
  Controller& operator=(const Controller& obj) = delete;
  Controller(Controller&& obj) noexcept
      : Geometry(obj._geometry_ptr, std::move(obj._devices)),
        default_sender_option(obj.default_sender_option),
        environment(obj.environment),
        _ptr(obj._ptr) {
    obj._geometry_ptr._0 = nullptr;
    obj._ptr._0 = nullptr;
  }
  Controller& operator=(Controller&& obj) noexcept {
    if (this != &obj) {
      this->_geometry_ptr = obj._geometry_ptr;
      this->_devices = std::move(obj._devices);
      this->default_sender_option = obj.default_sender_option;
      this->environment = obj.environment;
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
                           [&](const auto& d) { return native_methods::Quaternion{d.rot.w(), d.rot.x(), d.rot.y(), d.rot.z()}; });

    const auto ptr =
        validate(native_methods::AUTDControllerOpen(pos.data(), rot.data(), static_cast<uint16_t>(devices.size()), link.resolve(), option));
    auto geometry = AUTDGeometry(ptr);
    return Controller(geometry, ptr, option);
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

  AUTD3_API Sender sender(SenderOption option) const { return Sender(AUTDSender(_ptr, std::move(option)), geometry()); }

  template <driver::datagram D>
  AUTD3_API void send(const D& d) {
    sender(default_sender_option).send(d);
  }

  struct DefaultSenderOptionGetter {
    static SenderOption get(SenderOption option, native_methods::ControllerPtr) { return std::move(option); }
  };
  struct DefaultSenderOptionSetter {
    static void set(SenderOption& option, const native_methods::ControllerPtr ptr, const SenderOption& new_option) {
      option = new_option;
      AUTDSetDefaultSenderOption(ptr, new_option);
    }
  };

  native_methods::Property<SenderOption, native_methods::ControllerPtr, DefaultSenderOptionGetter, DefaultSenderOptionSetter> default_sender_option;
  Environment environment;

 private:
  AUTD3_API
  Controller(const native_methods::GeometryPtr geometry, const native_methods::ControllerPtr ptr, SenderOption option)
      : Geometry(geometry), default_sender_option(std::move(option), ptr), environment(Environment(AUTDEnvironment(ptr))), _ptr(ptr) {}

  native_methods::ControllerPtr _ptr;
};

}  // namespace autd3::controller
