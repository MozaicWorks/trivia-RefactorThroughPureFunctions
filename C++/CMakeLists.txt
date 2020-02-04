cmake_minimum_required(VERSION 3.12)
project(C__)

set(CMAKE_CXX_STANDARD 17)

include_directories(. gtest/include gmock/include)

LINK_DIRECTORIES(gtest gmock)

#LINK_LIBRARIES(gmock gtest pthread )


add_executable(C__
        Game.cpp
        Game.h
        GameRunner.cpp doctest.h GameTests.cpp TriviaGTests.cpp)
