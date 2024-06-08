target_sources(test_autd3 PRIVATE
  clear.cpp
  debug.cpp
  force_fan.cpp
  reads_fpga_state.cpp
  silencer.cpp
  synchronize.cpp
  segment.cpp
  pulse_width_encoder.cpp
  with_segment.cpp
  with_segment_transition.cpp
  with_timeout.cpp
  with_parallel_threshold.cpp
)

add_subdirectory(gain)
add_subdirectory(stm)
add_subdirectory(modulation)