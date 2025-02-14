target_sources(test_autd3 PRIVATE
  clear.cpp
  debug.cpp
  force_fan.cpp
  reads_fpga_state.cpp
  silencer.cpp
  synchronize.cpp
  segment.cpp
  pulse_width_encoder.cpp
  phase_corr.cpp
  with_segment.cpp
)

add_subdirectory(stm)
