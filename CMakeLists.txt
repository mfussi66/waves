cmake_minimum_required(VERSION 3.21)

project(waves)

  set(CMAKE_C_STANDARD 23)
  set(CMAKE_C_STANDARD_REQUIRED True)

  find_package(KissFFT QUIET)

  add_executable(${PROJECT_NAME})
  
  target_sources(${PROJECT_NAME}
  PRIVATE
  src/main.c  
  src/graphics.c
  src/wmath.c
  src/waves.c
  )

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE m sndfile alleg)

if(KissFFT_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE kissfft-float)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE kissfft)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    kiss_fft_scalar=double
)
