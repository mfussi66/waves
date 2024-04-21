cmake_minimum_required(VERSION 3.5)

project(
  waves
  LANGUAGES C)

  add_executable(
    waves 
    src/main.c 
    src/graphics.c 
    src/graphics.h 
    src/customdata.h
)

target_link_libraries(waves PRIVATE m kiss_fft_double sndfile alleg)
