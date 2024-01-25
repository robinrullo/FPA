cmake_minimum_required(VERSION 3.17)
project(FPA)

set(CMAKE_CXX_STANDARD 20)

if (NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif ()

add_executable(FPA main.cpp
        FPA.cpp
        FPA.h
        # Agent
        Agent.cpp
        Agent.h

        # Logger
        Logger.cpp
        Logger.h

        # Utils
        Levy.hpp
        randn.hpp
        randu.hpp
        randChoice.hpp
)
target_link_libraries(FPA PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

