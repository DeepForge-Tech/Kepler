cmake_minimum_required(VERSION 3.18)

project(Kepler)

set(CMAKE_CXX_STANDARD 20)

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(USE_HTTP "Use libraries for HTTP" ON)
option(BUILD_EXAMPLES "Build examples" ON)
include(FetchContent)

# Logger
FetchContent_Declare(Logger
  GIT_REPOSITORY https://github.com/DeepForge-Tech/Logger.git
  GIT_TAG master
  GIT_SHALLOW 1
)
# if(NOT Logger_POPULATED)
#   FetchContent_Populate(Logger)

#   add_subdirectory(${Logger_SOURCE_DIR} ${Logger_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(Logger)

# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/DeepForge-Technology/fmt.git
  GIT_TAG master
  GIT_SHALLOW 1
)
# if(NOT fmt_POPULATED)
#   FetchContent_Populate(fmt)

#   add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(fmt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES)

message("Use libraries for HTTP - ${USE_HTTP}")
if (USE_HTTP)
  
  set (Boost_NO_SYSTEM_PATHS ON)
  set (Boost_USE_MULTITHREADED ON)
  set (Boost_USE_STATIC_LIBS ON)
  set (BOOST_ALL_DYN_LINK OFF)
  find_package(Boost REQUIRED COMPONENTS system json)

  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND SOURCE_FILES
    src/HttpServer/DefaultMiddlewares.cpp
    src/HttpServer/HttpRequest.cpp
    src/HttpServer/HttpResponse.cpp
    src/HttpServer/HttpServer.cpp
    src/HttpServer/Middleware.cpp
    src/HttpServer/Router.cpp
    src/HttpServer/Session.cpp
  )
endif()

# if (BUILD_SHARED_LIBS)
#     add_library(Kepler SHARED
#       ${SOURCE_FILES}
#       # src/main.cpp
#     )
# else()
    add_library(Kepler STATIC
      ${SOURCE_FILES}
    )
# endif()
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set_property(TARGET Kepler PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(Kepler 
    ${Boost_LIBRARIES}
    Logger
    fmt
  )
# find_package(OpenSSL REQUIRED)
# OpenSSL::SSL
# OpenSSL::Crypto

# Build examples
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)