#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Wav final : public driver::Modulation<Wav> {
 public:
  AUTD3_API explicit Wav(std::filesystem::path path) : Modulation(driver::SamplingConfig::Division(5120)), _path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override {
    return validate(AUTDModulationWav(_path.string().c_str(), _config, _loop_behavior));
  }

 private:
  std::filesystem::path _path;
};

}  // namespace autd3::modulation::audio_file
