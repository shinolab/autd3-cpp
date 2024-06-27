#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Wav final : public driver::ModulationBase<Wav>,
                  public driver::IntoModulationCache<Wav>,
                  public driver::IntoRadiationPressure<Wav>,
                  public driver::IntoModulationTransform<Wav> {
 public:
  AUTD3_API explicit Wav(std::filesystem::path path) : _path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(AUTDModulationAudioFileWav(_path.string().c_str(), _loop_behavior));
  }

 private:
  std::filesystem::path _path;
};

}  // namespace autd3::modulation::audio_file
