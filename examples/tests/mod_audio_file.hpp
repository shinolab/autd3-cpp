#pragma once

#include <filesystem>

#include "autd3.hpp"
#include "autd3/modulation/audio_file.hpp"

namespace fs = std::filesystem;

inline void mod_audio_file_test(autd3::Controller& autd) {
  autd.send(autd3::Silencer());

  const fs::path path = fs::path(AUTD3_RESOURCE_PATH).append("sin150.wav");

  const autd3::Point3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);

  autd.send((autd3::modulation::audio_file::Wav(path), autd3::Focus(center, autd3::FocusOption{})));
}
