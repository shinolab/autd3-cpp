target_sources(test_autd3 PRIVATE
  clear.cpp
  gpio_out.cpp
  force_fan.cpp
  group.cpp
  reads_fpga_state.cpp
  silencer.cpp
  synchronize.cpp
  segment.cpp
  pulse_width_encoder.cpp
  phase_corr.cpp
  with_segment.cpp
  output_mask.cpp
)

add_subdirectory(stm)
