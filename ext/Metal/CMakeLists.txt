cmake_minimum_required(VERSION 3.20)

# Library definition
add_library(METAL_API
    ${CMAKE_CURRENT_SOURCE_DIR}/define.cpp
)

# Metal cpp headers
target_include_directories(METAL_API PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp"
    )

# Metal cpp library (linker)
target_link_libraries(METAL_API
    "-framework Metal"
    "-framework MetalKit"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore"
)