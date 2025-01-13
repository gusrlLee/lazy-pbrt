cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(lazy-pbrt)

find_package(embree REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(ext)

add_executable(${PROJECT_NAME} 
    main.cpp

    core/camera.h
    core/camera.cpp
    core/color.h
    core/hittable.h
    core/hitrecord.h
    core/hittablelist.h
    core/hittablelist.cpp
    core/interval.h
    core/interval.cpp
    core/material.h
    core/random.h
    core/random.cpp
    core/ray.h
    core/scene.h
    core/types.h
    core/image.h
    core/light.h
    core/onb.h

    integrator/integrator.h
    integrator/path.h
    integrator/path.cpp

    material/lambertian.h
    material/lambertian.cpp
    material/metal.h
    material/metal.cpp
    material/dielectric.h
    material/dielectric.cpp
    material/constantmedium.h
    material/constantmedium.cpp
    material/isotropic.h

    shapes/sphere.h
    shapes/sphere.cpp
    shapes/quad.h
    shapes/quad.cpp

    math/vec3.h
    math/aabb.h
    math/aabb.cpp
    math/perlin.h
    math/perlin.cpp
    math/pdf.h

    lights/diffuse.h

    accel/bvh.h
    accel/bvh.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext)

target_link_libraries(${PROJECT_NAME} METAL_API)
target_link_libraries(${PROJECT_NAME} embree)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
target_link_libraries(${PROJECT_NAME} STB_LIB)
