#pragma once

#include <array>

#include "autd3/driver/firmware/fpga/pulse_width.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::controller {
class Controller;
}

namespace autd3::link {

struct Audit final {
  friend class controller::Controller;

  native_methods::LinkPtr _ptr;

  Audit() : _ptr(native_methods::LinkPtr{nullptr}) {}

  [[nodiscard]] native_methods::LinkPtr resolve() { return native_methods::AUTDLinkAudit(); }

  [[nodiscard]] bool is_open() const { return AUTDLinkAuditIsOpen(_ptr); }

  [[nodiscard]] bool is_force_fan(const size_t idx) const { return AUTDLinkAuditFpgaIsForceFan(_ptr, static_cast<uint16_t>(idx)); }

  void break_down() const { AUTDLinkAuditBreakDown(_ptr); }
  void repair() const { AUTDLinkAuditRepair(_ptr); }

  [[nodiscard]] uint16_t silencer_update_rate_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRateIntensity(_ptr, static_cast<uint16_t>(idx));
  }
  [[nodiscard]] uint16_t silencer_update_rate_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRatePhase(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t silencer_completion_steps_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsIntensity(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t silencer_completion_steps_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsPhase(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool silencer_fixed_completion_steps_mode(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerFixedCompletionStepsMode(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool silencer_strict(const size_t idx) const { return AUTDLinkAuditCpuSilencerStrict(_ptr, static_cast<uint16_t>(idx)); }

  [[nodiscard]] std::array<uint8_t, 4> debug_types(const size_t idx) const {
    std::array<uint8_t, 4> ty{};
    AUTDLinkAuditFpgaGPIOOutputTypes(_ptr, static_cast<uint16_t>(idx), ty.data());
    return ty;
  }

  [[nodiscard]] std::array<uint64_t, 4> debug_values(const size_t idx) const {
    std::array<uint64_t, 4> value{};
    AUTDLinkAuditFpgaDebugValues(_ptr, static_cast<uint16_t>(idx), value.data());
    return value;
  }

  void assert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaAssertThermalSensor(_ptr, static_cast<uint16_t>(idx)); }

  void deassert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaDeassertThermalSensor(_ptr, static_cast<uint16_t>(idx)); }

  [[nodiscard]] std::vector<uint8_t> modulation(const size_t idx, const native_methods::Segment segment) const {
    const auto n = AUTDLinkAuditFpgaModulationCycle(_ptr, segment, static_cast<uint16_t>(idx));
    std::vector<uint8_t> buf(n);
    AUTDLinkAuditFpgaModulationBuffer(_ptr, segment, static_cast<uint16_t>(idx), buf.data());
    return buf;
  }  // LCOV_EXCL_LINE

  [[nodiscard]] uint32_t modulation_freq_divide(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationFreqDivide(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t modulation_loop_count(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationLoopCount(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] std::vector<native_methods::Drive> drives(const size_t idx, const native_methods::Segment segment, const int stm_idx) const {
    const auto n = AUTDLinkAuditCpuNumTransducers(_ptr, static_cast<uint16_t>(idx));
    std::vector<native_methods::Drive> drives(n);
    AUTDLinkAuditFpgaDrivesAt(_ptr, segment, static_cast<uint16_t>(idx), static_cast<uint16_t>(stm_idx), drives.data());
    return drives;
  }  // LCOV_EXCL_LINE

  [[nodiscard]] uint32_t stm_cycle(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmCycle(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool is_stm_gain_mode(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaIsStmGainMode(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint32_t stm_freq_divide(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmFreqDivide(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t stm_loop_count(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmLoopCount(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::Segment current_stm_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentStmSegment(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint32_t stm_sound_speed(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaSoundSpeed(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::Segment current_mod_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentModSegment(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] std::vector<driver::PulseWidth> pulse_width_encoder_table(const size_t idx) const {
    std::vector<driver::PulseWidth> buf;
    buf.resize(256, driver::PulseWidth(0));
    AUTDLinkAuditFpgaPulseWidthEncoderTable(_ptr, static_cast<uint16_t>(idx), reinterpret_cast<uint64_t*>(buf.data()));
    return buf;
  }  // LCOV_EXCL_LINE

 private:
  explicit Audit(const native_methods::LinkPtr ptr) : _ptr(ptr) {}
};

}  // namespace autd3::link
