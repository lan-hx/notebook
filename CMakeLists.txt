cmake_minimum_required(VERSION 3.21)
project(notebook)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}"
)

add_executable(notebook main.cpp src/Client.cpp src/Server.cpp src/Editor.cpp src/ClientCon.cpp src/DB.cpp src/DBTrans.cpp)
