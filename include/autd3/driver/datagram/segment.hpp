#pragma once

#include <chrono>
#include <ranges>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class ChangeGainSTMSegment final {
 public:
  explicit ChangeGainSTMSegment(const native_methods::Segment segment) : _segment(segment){};

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramChangeGainSTMSegment(_segment); }

 private:
  native_methods::Segment _segment;
};

class ChangeFocusSTMSegment final {
 public:
  explicit ChangeFocusSTMSegment(const native_methods::Segment segment) : _segment(segment){};

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramChangeFocusSTMSegment(_segment); }

 private:
  native_methods::Segment _segment;
};

class ChangeGainSegment final {
 public:
  explicit ChangeGainSegment(const native_methods::Segment segment) : _segment(segment){};

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramChangeGainSegment(_segment); }

 private:
  native_methods::Segment _segment;
};

class ChangeModulationSegment final {
 public:
  explicit ChangeModulationSegment(const native_methods::Segment segment) : _segment(segment){};

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramChangeModulationSegment(_segment); }

 private:
  native_methods::Segment _segment;
};

}  // namespace autd3::driver