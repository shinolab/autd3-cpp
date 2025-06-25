target_sources(test_autd3 PRIVATE
  fourier.cpp
  sine.cpp
  square.cpp
  static.cpp
  custom.cpp  
  radiation_pressure.cpp
  fir.cpp
)

add_subdirectory(audio_file)
