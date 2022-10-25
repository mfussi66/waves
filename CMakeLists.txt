cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")

# set the project name
project(waves LANGUAGES C)

find_package(kissfft REQUIRED)

# `sources` and `data`
file(GLOB_RECURSE sources      src/waves/*.c src/waves/*.h)
file(GLOB_RECURSE exern_sources src/extern/kissfft/kiss_fft.c src/extern/kissfft/kiss_fft.h)

# add the executable
add_executable(waves ${sources} ${exern_sources})

target_compile_options(waves PUBLIC -std=c11 -Wall -Wfloat-conversion)

find_package(PkgConfig REQUIRED)

pkg_check_modules(ALLEGRO5 REQUIRED allegro-5)
target_link_libraries(waves ${ALLEGRO5_LIBRARIES})
target_include_directories(waves PUBLIC ${ALLEGRO5_INCLUDE_DIRS})

target_link_libraries(waves m ${ITK_LIBRARIES})
