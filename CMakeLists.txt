cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(pt)
find_package(embree REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} 
    main.cpp
    vec3.h
    types.h

    color.h
    color.cpp
    
    sphere.h
    sphere.cpp
    
    world.h
    world.cpp

    interval.h
    interval.cpp

    camera.h
    camera.cpp

    material.h
    material.cpp
)

target_link_libraries(${PROJECT_NAME} embree)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
