#pragma once

#include <string>

#include "autd3/controller/controller.hpp"
#include "autd3/def.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/common/angle.hpp"
#include "autd3/driver/common/freq.hpp"
#include "autd3/driver/datagram/clear.hpp"
#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/datagram/force_fan.hpp"
#include "autd3/driver/datagram/gpio_out.hpp"
#include "autd3/driver/datagram/group.hpp"
#include "autd3/driver/datagram/output_mask.hpp"
#include "autd3/driver/datagram/phase_corr.hpp"
#include "autd3/driver/datagram/pulse_width_encoder.hpp"
#include "autd3/driver/datagram/reads_fpga_state.hpp"
#include "autd3/driver/datagram/segment.hpp"
#include "autd3/driver/datagram/silencer.hpp"
#include "autd3/driver/datagram/stm/foci.hpp"
#include "autd3/driver/datagram/stm/gain.hpp"
#include "autd3/driver/datagram/synchronize.hpp"
#include "autd3/driver/datagram/with_finite_loop.hpp"
#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/firmware/fpga/pulse_width.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/rotation.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/gain/bessel.hpp"
#include "autd3/gain/focus.hpp"
#include "autd3/gain/group.hpp"
#include "autd3/gain/null.hpp"
#include "autd3/gain/plane.hpp"
#include "autd3/gain/uniform.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/modulation/square.hpp"
#include "autd3/modulation/static.hpp"

namespace autd3 {

constexpr float pi = driver::pi;

static inline std::string version = "37.0.0";

using autd3::driver::deg;
using autd3::driver::Freq;
using autd3::driver::Hz;
using autd3::driver::kHz;
using autd3::driver::rad;

using driver::AUTD3;
using driver::geometry::Device;
using driver::geometry::EulerAngles;
using driver::geometry::Geometry;
using driver::geometry::Transducer;

using driver::DcSysTime;
using driver::Drive;
using driver::GPIOOutputType;
using driver::Intensity;
using driver::Phase;
using driver::PulseWidth;
using driver::SamplingConfig;
using native_methods::GainSTMMode;
using native_methods::GPIOIn;
using native_methods::GPIOOut;
using native_methods::Segment;

namespace transition_mode {
using driver::transition_mode::Ext;
using driver::transition_mode::GPIO;
using driver::transition_mode::Immediate;
using driver::transition_mode::Later;
using driver::transition_mode::SyncIdx;
using driver::transition_mode::SysTime;
}  // namespace transition_mode

using driver::Clear;
using driver::ControlPoint;
using driver::ControlPoints;
using driver::DatagramTuple;
using driver::FixedCompletionSteps;
using driver::FixedCompletionTime;
using driver::FixedUpdateRate;
using driver::FociSTM;
using driver::ForceFan;
using driver::Gain;
using driver::GainSTM;
using driver::GainSTMOption;
using driver::GPIOOutputs;
using driver::GPIOOutputType;
using driver::Group;
using driver::OutputMask;
using driver::PhaseCorrection;
using driver::PulseWidthEncoder;
using driver::ReadsFPGAState;
using driver::Silencer;
using driver::SwapSegmentFociSTM;
using driver::SwapSegmentGain;
using driver::SwapSegmentGainSTM;
using driver::SwapSegmentModulation;
using driver::Synchronize;
using driver::WithFiniteLoop;
using driver::WithSegment;

using native_methods::GainSTMMode;

using gain::Bessel;
using gain::BesselOption;
using gain::Focus;
using gain::FocusOption;
using gain::GainGroup;
using gain::Null;
using gain::Plane;
using gain::PlaneOption;
using gain::Uniform;

using modulation::Sine;
using modulation::SineOption;
using modulation::Square;
using modulation::SquareOption;
using modulation::Static;

using driver::FirmwareVersion;
using driver::FPGAState;

using driver::Point3;
using driver::Quaternion;
using driver::Vector3;

using controller::Controller;
using controller::SenderOption;

}  // namespace autd3
