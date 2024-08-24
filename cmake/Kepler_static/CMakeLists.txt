cmake_minimum_required(VERSION 3.18)

if (WIN32)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
# set(VCPKG_CRT_LINKAGE static)
# set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "VCPKG Target Triplet to use")
endif()
# set(BOOST_ROOT "C:/Boost/")
project(Kepler_static)

set(BUILD_SHARED_LIBS OFF)

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_ALL_DYN_LINK OFF)

set( _boost_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if(WIN32)
  list(INSERT CMAKE_FIND_LIBRARY_SUFFIXES 0 .lib .a)
elseif(UNIX)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
elseif(APPLE)
set(CMAKE_FIND_LIBRARY_SUFFIXES .dylib)
endif()

include(../Source.cmake)

if (USE_HTTP)
  find_package(Boost REQUIRED COMPONENTS system coroutine context)

  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND HTTP_SERVER_LIBRARIES  ${Boost_LIBRARIES} Logger)
  list(APPEND LIBRARIES ${HTTP_SERVER_LIBRARIES})
endif()

if (USE_WEBCLIENT)
  find_package(OpenSSL REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system coroutine context)
  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto cppcoro)
endif()

list(APPEND LIBRARIES fmt jsoncpp_static)

add_library(Kepler_static STATIC
  ${SOURCE_FILES}
)

target_link_libraries(Kepler_static 
  ${LIBRARIES}
)