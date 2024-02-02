cmake_minimum_required(VERSION 3.17)
project(FPA)

set(CMAKE_CXX_STANDARD 20)

add_executable(FPA main.cpp
        FPA.cpp
        FPA.h
        # Agent
        Agent.cpp
        Agent.h
        # Utils
        Levy.hpp
        randn.hpp
        randu.hpp
        randChoice.hpp
)
