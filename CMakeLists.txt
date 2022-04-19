# author: lan

cmake_minimum_required(VERSION 3.1)
# written only for author, please ignore it
if (WIN32 AND VCPKG_DIR STREQUAL "" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(VCPKG_DIR "D:/vcpkg")
    message("Assuming vcpkg in dir ${VCPKG_DIR}")
endif ()

if(DEFINED VCPKG_DIR AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif()

#if(DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VCPKG_TARGET_TRIPLET)
#    set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
#    message("TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")
#endif()

project(notebook)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SLOW "slow")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wmisleading-indentation -Wunused -Wconversion -Wshadow -Wcast-qual -Wcast-align -Wunreachable-code -fno-exceptions -fno-rtti -march=native")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10.0)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fanalyzer")
    endif()
    if(DEFINED SLOW)
        string(REGEX REPLACE " -fno-exceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
        # string(REGEX REPLACE " -fno-rtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Weffc++ -fstack-protector-all -D_GLIBCXX_ASSERTIONS")
    if(NOT WIN32)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
    endif()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")
    if (WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=GBK")
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # warning
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    # disable exceptions and RTTI
    string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /fp:fast /GA /Gd /GL /GR- /GS /guard:cf /permissive- /sdl /Zc:__cplusplus /Zc:forScope /Zc:inline /Zc:rvalueCast /arch:AVX2 /std:c++latest /MP /experimental:module /source-charset:utf-8 /execution-charset:GBK")
    if(DEFINED SLOW)
        string(REGEX REPLACE "/GR-" "/GR" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    endif()
    # /fsanitize=address
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /RTC1 /analyze")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zi /Zo")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOLOGO /LTCG:incremental /MANIFEST /OPT:REF /DYNAMICBASE /NXCOMPAT /DEBUG")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

include_directories(
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}"
)

add_executable(notebook src/main.cpp src/Client.cpp src/Server.cpp src/Editor.cpp src/ClientCon.cpp src/DB.cpp src/DBTrans.cpp src/ServerCon.cpp src/ClientTrans.cpp)

include_directories(SYSTEM "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

add_library(nb_lib STATIC src/Client.cpp src/Server.cpp src/Editor.cpp src/DB.cpp src/DBTrans.cpp src/ClientTrans.cpp)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(notebook PRIVATE unofficial::sqlite3::sqlite3)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS REQUIRED)
#target_link_libraries(learningcpp PRIVATE Boost::dynamic_linking)
if (Boost_FOUND)
    message("boost include: ${Boost_INCLUDE_DIRS}")
    message("boost lib: ${Boost_LIBRARIES}")
    #target_include_directories(learningcpp PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(notebook PRIVATE)
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_VERBOSE_MAKEFILE on)

enable_testing()
add_subdirectory(test)