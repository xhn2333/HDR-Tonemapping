set(_HDR_RS_SOURCE_DIR "${hdrplus_SOURCE_DIR}/hdr-rs")
set(_HDR_RS_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/release/${CMAKE_STATIC_LIBRARY_PREFIX}hdr_rs${CMAKE_STATIC_LIBRARY_SUFFIX}")
file(GLOB_RECURSE _HDR_RS_SRCS "${_HDR_RS_SOURCE_DIR}/src/*.rs")
file(GLOB_RECURSE _HDR_RS_HEADERS "${_HDR_RS_SOURCE_DIR}/include/*.h")
add_custom_command(OUTPUT ${_HDR_RS_LIBRARY}
        COMMENT "Building symbolization"
        COMMAND cargo build --release --target-dir ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
        WORKING_DIRECTORY ${_HDR_RS_SOURCE_DIR}
        DEPENDS "${_HDR_RS_SRCS}"
        "${_HDR_RS_HEADERS}"
        "${_HDR_RS_SOURCE_DIR}/Cargo.toml")
add_custom_target(hdr-rs-cargo ALL DEPENDS ${_HDR_RS_LIBRARY})
add_library(hdr-rs STATIC IMPORTED GLOBAL)
set_target_properties(hdr-rs PROPERTIES IMPORTED_LOCATION ${_HDR_RS_LIBRARY})
add_dependencies(hdr-rs hdr-rs-cargo)
target_include_directories(hdr-rs INTERFACE ${_HDR_RS_SOURCE_DIR}/include)