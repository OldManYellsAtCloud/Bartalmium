cmake_minimum_required(VERSION 3.5)

project(7segment-user-driver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(7segment-user-driver main.cpp)

include(GNUInstallDirs)
install(TARGETS 7segment-user-driver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
