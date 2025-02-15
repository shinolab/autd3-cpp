#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

struct CsvOption {
  uint8_t delimiter = ',';
};

struct Csv final : driver::Modulation, driver::IntoDatagramTuple<Csv> {
  template <driver::sampling_config T>
  AUTD3_API explicit Csv(std::filesystem::path path, const T sampling_config, const CsvOption option)
      : path(std::move(path)), config(sampling_config), option(option) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(native_methods::AUTDModulationAudioFileCsv(path.string().c_str(), config, option.delimiter));
  }

  std::filesystem::path path;
  driver::SamplingConfig config;
  CsvOption option;
};

}  // namespace autd3::modulation::audio_file
