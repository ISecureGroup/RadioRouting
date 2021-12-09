cmake_minimum_required(VERSION 3.20)
project(RadioRouting C)

set(CMAKE_C_STANDARD 99)

add_executable(RadioRouting core/main.c core/protocol/protocol.h core/protocol/protocol.c tests/paxample.h tests/showme.h tests/source/showme.c core/protocol/controller/controller.h core/protocol/controller/controller.c core/protocol/factorys/factorys.c core/protocol/factorys/factorys.h core/protocol/handlers/handlers.c core/protocol/handlers/handlers.h)
