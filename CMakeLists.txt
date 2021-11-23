cmake_minimum_required(VERSION 3.20)
project(RadioRouting C)

set(CMAKE_C_STANDARD 99)

add_executable(RadioRouting core/main.c core/protocol.h core/protocol.c tests/paxample.h tests/showme.h tests/source/showme.c)
