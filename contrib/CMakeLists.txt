find_package(Boost REQUIRED COMPONENTS program_options system)
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

message(STATUS "OpenMP Configuration: ${OpenMP_CXX_INCLUDE_DIRS}:${OpenMP_CXX_LIBRARIES}")
message(STATUS "Boost Configuration: ${Boost_INCLUDE_DIRS}:${Boost_LIBRARIES}")

set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "abseil-cpp" FORCE)
set(ABSL_USE_EXTERNAL_GOOGLETEST BOOL "abseil-cpp" FORCE)
add_subdirectory(abseil-cpp)
add_subdirectory(snmalloc)
add_subdirectory(googletest)

add_library(hdr-support INTERFACE)
target_link_libraries(
        hdr-support
        INTERFACE
        absl::btree
        absl::flat_hash_map
        absl::strings
        snmallocshim
)

target_compile_options(hdr-support INTERFACE ${OpenMP_CXX_FLAGS})
target_include_directories(hdr-support INTERFACE ${Boost_INCLUDE_DIRS} ${OpenMP_CXX_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(hdr-support INTERFACE ${Boost_LIBRARIES} ${OpenMP_CXX_LIBRARIES} ${OpenCV_LIBS})
