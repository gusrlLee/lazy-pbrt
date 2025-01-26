cmake_minimum_required(VERSION 3.30)

project(gi)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_subdirectory(ext)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/ext)


target_link_libraries(${PROJECT_NAME} tinyobjloader)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)

