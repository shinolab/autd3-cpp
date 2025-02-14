#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Wav final : public driver::Modulation, public driver::IntoDatagramTuple<Wav> {
 public:
  AUTD3_API explicit Wav(std::filesystem::path path) : path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(native_methods::AUTDModulationAudioFileWav(path.string().c_str()));
  }

  std::filesystem::path path;
};

}  // namespace autd3::modulation::audio_file
