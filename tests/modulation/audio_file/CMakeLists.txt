target_sources(test_autd3 PRIVATE
  wav.cpp
  csv.cpp
)

target_compile_definitions(test_autd3 PRIVATE AUTD3_TEST_AUDIOFILE_RESOURCE_PATH="${PROJECT_SOURCE_DIR}/modulation/audio_file")
