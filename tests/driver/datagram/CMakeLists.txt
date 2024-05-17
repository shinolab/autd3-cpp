target_sources(test_autd3 PRIVATE
  clear.cpp
  debug.cpp
  force_fan.cpp
  reads_fpga_state.cpp
  silencer.cpp
  synchronize.cpp
  phase_filter.cpp
  segment.cpp
  pulse_width_encoder.cpp
  with_segment.cpp
  with_segment_transition.cpp
)

add_subdirectory(gain)
add_subdirectory(stm)
add_subdirectory(modulation)