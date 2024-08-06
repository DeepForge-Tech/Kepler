cmake_minimum_required(VERSION 3.18)
if (WIN32)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
project(Kepler)

set(CMAKE_CXX_STANDARD 20)

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(USE_HTTP "Use libraries for HTTP" ON)
option(KEPLER_BUILD_EXAMPLES "Build examples" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadDependecies.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HTTP_SERVER_SOURCE_FILES)

message("Use libraries for HTTP - ${USE_HTTP}")
if (USE_HTTP)
  
  set (Boost_NO_SYSTEM_PATHS ON)
  set (Boost_USE_MULTITHREADED ON)
  set (Boost_USE_STATIC_LIBS ON)
  set (BOOST_ALL_DYN_LINK OFF)
  find_package(Boost REQUIRED COMPONENTS system json)

  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND HTTP_SERVER_SOURCE_FILES
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
#       ${HTTP_SERVER_SOURCE_FILES}
#       # src/main.cpp
#     )
# else()
add_library(Kepler SHARED
  ${HTTP_SERVER_SOURCE_FILES}
)

add_library(Kepler_static STATIC
  ${HTTP_SERVER_SOURCE_FILES}
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
if (KEPLER_BUILD_EXAMPLES)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()