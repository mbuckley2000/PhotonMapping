cmake_minimum_required(VERSION 3.12)
project(ACG_MiniCW_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(ACG-MiniCW-3)

add_executable(ACG_MiniCW_3
        ACG-MiniCW-3/ACG-MiniCW-3.cpp
        ACG-MiniCW-3/Camera.cpp
        ACG-MiniCW-3/Camera.h
        ACG-MiniCW-3/CW2-recap.cpp
        ACG-MiniCW-3/DirectionalLight.cpp
        ACG-MiniCW-3/DirectionalLight.h
        ACG-MiniCW-3/Light.h
        ACG-MiniCW-3/Mesh.cpp
        ACG-MiniCW-3/Mesh.h
        ACG-MiniCW-3/Object.h
        ACG-MiniCW-3/pch.cpp
        ACG-MiniCW-3/pch.h
        ACG-MiniCW-3/Plane.cpp
        ACG-MiniCW-3/Ray.cpp
        ACG-MiniCW-3/Ray.h
        ACG-MiniCW-3/resource.h
        ACG-MiniCW-3/Scene.cpp
        ACG-MiniCW-3/Scene.h
        ACG-MiniCW-3/Sphere.cpp
        ACG-MiniCW-3/Sphere.h
        ACG-MiniCW-3/Triangle.cpp
        ACG-MiniCW-3/Triangle.h)
