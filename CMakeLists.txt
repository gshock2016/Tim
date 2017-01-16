cmake_minimum_required(VERSION 3.6)
project(TIM__release_v1_2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/sfmt/a.c
        src/sfmt/a.cpp
        src/sfmt/SFMT-alti.h
        src/sfmt/SFMT-common.h
        src/sfmt/SFMT-params.h
        src/sfmt/SFMT-params11213.h
        src/sfmt/SFMT-params1279.h
        src/sfmt/SFMT-params132049.h
        src/sfmt/SFMT-params19937.h
        src/sfmt/SFMT-params216091.h
        src/sfmt/SFMT-params2281.h
        src/sfmt/SFMT-params4253.h
        src/sfmt/SFMT-params44497.h
        src/sfmt/SFMT-params607.h
        src/sfmt/SFMT-params86243.h
        src/sfmt/SFMT-sse2.h
        src/sfmt/SFMT.c
        src/sfmt/SFMT.h
        src/sfmt/test.c
        src/graph.h
        src/head.h
        src/infgraph.h
        src/memoryusage.h
        src/tim.cpp
        src/timgraph.h)

add_executable(TIM__release_v1_2 ${SOURCE_FILES})