cmake_minimum_required(VERSION 3.18)
project(MultithreadHttpServer)

option(USE_FETCH_CONTENT "Download and compile Kepler from source with FetchContent" OFF)

set(SOURCE_FILES src/main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (USE_FETCH_CONTENT)
include(FetchContent)
# Kepler
# FetchContent_Declare(Kepler
#   GIT_REPOSITORY https://github.com/DeepForge-Tech/Kepler.git
#   GIT_TAG master
#   GIT_SHALLOW 1
# )
# # if(NOT Kepler_POPULATED)
# #   FetchContent_Populate(Kepler)

# #   add_subdirectory(${Kepler_SOURCE_DIR} ${Kepler_BINARY_DIR} EXCLUDE_FROM_ALL)
# # endif()
# FetchContent_MakeAvailable(Kepler)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Kepler_static)