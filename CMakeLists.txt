cmake_minimum_required(VERSION 3.23)
project(hdrplus)

set(CMAKE_CXX_STANDARD 20)


option(ENABLE_AMDGPU_OFFLOAD "Enable AMDGPU Offload" OFF)
option(ENABLE_CUDA_OFFLOAD "Enable CUDA Offload" OFF)

include_directories(include)

add_subdirectory(contrib)
add_library(hdrplus SHARED src/hdr.cpp src/ToneMapping.cpp include/hdr/ToneMapping.h)

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_options(hdrplus PRIVATE -march=native -mfma -Ofast -ffast-math -flto)
    target_link_options(hdrplus PRIVATE -flto)
    message(STATUS "Set full optimization: ${CMAKE_CXX_FLAGS}")
endif()

target_link_libraries(hdrplus PUBLIC hdr-support)

if (ENABLE_AMDGPU_OFFLOAD)
    set(CMAKE_CXX_FLAGS "-fopenmp=libomp -fopenmp-targets=amdgcn ${CMAKE_CXX_FLAGS}")
    target_compile_options(hdrplus PUBLIC -rtlib=libgcc -unwindlib=libgcc -stdlib=libstdc++)
    target_link_libraries(hdrplus PUBLIC omptarget.rtl.amdgpu omptarget -rtlib=libgcc -unwindlib=libgcc -stdlib=libstdc++)
endif ()

if (ENABLE_CUDA_OFFLOAD)
    set(CMAKE_CXX_FLAGS "-fopenmp=libomp -fopenmp-targets=nvptx64 ${CMAKE_CXX_FLAGS}")
    target_compile_options(hdrplus PUBLIC -rtlib=libgcc -unwindlib=libgcc -stdlib=libstdc++)
    target_link_libraries(hdrplus PUBLIC omptarget.rtl.cuda omptarget -rtlib=libgcc -unwindlib=libgcc -stdlib=libstdc++)
endif ()


add_subdirectory(test)
add_subdirectory(hdr-rs)
target_link_libraries(hdrplus PUBLIC hdr-rs)