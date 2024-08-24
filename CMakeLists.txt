cmake_minimum_required(VERSION 3.18)
option(USE_KEPLER_STATIC "Build static library" ON)
option(USE_RUNTIME_STATIC "Build static runtime" ON)
# set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if (USE_KEPLER_STATIC)
    set(BOOST_ROOT "C:/Library/lib/boost-static-lib/")
    set(Boost_USE_STATIC_LIBS ON)
    set(BOOST_ALL_DYN_LINK OFF)
    set(BUILD_STATIC_LIBS ON)
    set(BUILD_SHARED_LIBS OFF)
else()
  set(BOOST_ROOT "C:/Library/boost-dynamic-lib/")
  set(BUILD_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_LIBS OFF)
    set(BOOST_ALL_DYN_LINK ON)
    set(BUILD_SHARED_LIBS ON)
endif()

if (USE_RUNTIME_STATIC)
  if (MSVC)
    add_compile_options(
      $<$<CONFIG:>:/MT> #---------|
      $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
      $<$<CONFIG:Release>:/MT> #--|
    )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a .lib")
    elseif(UNIX OR APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a .lib")
    elseif(UNIX OR APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-static")
  endif()
  set(BOOST_RUNTIME_LINK "static")
  set(Boost_USE_STATIC_RUNTIME ON)
else()
  if (MSVC)
    add_compile_options(
      $<$<CONFIG:>:/MD> #---------|
      $<$<CONFIG:Debug>:/MDd> #---|-- Dynamically link the runtime libraries
      $<$<CONFIG:Release>:/MD> #--|
    )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a .lib")
    elseif(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    elseif(APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
    elseif(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    elseif(APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-static")
  endif()
  set(BOOST_RUNTIME_LINK "shared")
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

project(Kepler)

add_definitions(-D_WIN32_WINNT=0x0601)
# cmake_policy(SET CMP0144 NEW)
# cmake_policy(SET CMP0135 NEW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(USE_HTTP "Use libraries for HTTP" ON)
option(KEPLER_BUILD_EXAMPLES "Build examples" OFF)
option(USE_WEBCLIENT "Use libraries for http/https web client" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadDependecies.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(HTTP_SERVER_SOURCE_FILES)
set(WEBCLIENT_SOURCE_FILES)
set(SOURCE_FILES)

# Libraries
set(HTTP_SERVER_LIBRARIES)
set(WEBCLIENT_LIBRARIES)
set(LIBRARIES)

# set(Boost_NO_SYSTEM_PATHS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_DEBUG_LIBS OFF)
# set(Boost_USE_RELEASE_LIBS ON)

set(BOOST_INCLUDE_LIBRARIES system coroutine context asio beast)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.84.0
  GIT_SHALLOW 1
)
FetchContent_MakeAvailable(Boost)

include_directories(${BOOST_LIBRARY_INCLUDES})
include_directories(asio INTERFACE ${boost_asio_SOURCE_DIR}/include)
include_directories(coroutine INTERFACE ${boost_coroutine_SOURCE_DIR}/include)
include_directories(system INTERFACE ${boost_system_SOURCE_DIR}/include)
include_directories(context INTERFACE ${boost_context_SOURCE_DIR}/include)
include_directories(beast INTERFACE ${boost_beast_SOURCE_DIR}/include)
# Add source to this project's executable.
# add_executable (ComUdpProxy "ComUdpProxy.cpp" "ComUdpProxy.h" "Sender.h"  "Receiver.h" "Message.h")

#target_include_directories(ComUdpProxy PRIVATE ${BOOST_LIBRARY_INCLUDES})

message("Use libraries for HTTP - ${USE_HTTP}")
if (USE_HTTP)
  # find_package(Boost REQUIRED COMPONENTS system coroutine context asio beast)

  # include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND HTTP_SERVER_SOURCE_FILES
    src/HttpServer/DefaultMiddlewares.cpp
    src/HttpServer/HttpRequest.cpp
    src/HttpServer/HttpResponse.cpp
    src/HttpServer/HttpServer.cpp
    src/HttpServer/Middleware.cpp
    src/HttpServer/Router.cpp
    src/HttpServer/Session.cpp
  )
  list(APPEND SOURCE_FILES ${HTTP_SERVER_SOURCE_FILES})
  list(APPEND HTTP_SERVER_LIBRARIES  Boost::asio Boost::system Boost::context Boost::coroutine Boost::beast Logger)
  list(APPEND LIBRARIES ${HTTP_SERVER_LIBRARIES})
endif()

if (USE_WEBCLIENT)
  find_package(OpenSSL REQUIRED)
  # find_package(Boost REQUIRED COMPONENTS system coroutine context asio beast)
  # find_package(cppcoro REQUIRED)
  # include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND WEBCLIENT_SOURCE_FILES src/WebClient/WebClient.cpp src/HttpClient/HttpClient.cpp)
  list(APPEND LIBRARIES cppcoro Boost::asio Boost::system Boost::context Boost::coroutine Boost::beast OpenSSL::SSL OpenSSL::Crypto)
  list(APPEND SOURCE_FILES ${WEBCLIENT_SOURCE_FILES})
endif()

# if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#     # set(CMAKE_CXX_FLAGS_DEBUG
#     #         "/DWIN32 /D_WINDOWS /EHsc /WX /wd4355 /wd4251 /wd4250 /wd4996 /FS"
#     #         CACHE STRING "Debug compiler flags" FORCE
#     # )
#     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /MTd")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_ITERATOR_DEBUG_LEVEL=2")
#     # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601 -D_ITERATOR_DEBUG_LEVEL=0 -MT")
#     # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_ITERATOR_DEBUG_LEVEL=0")
#     # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_ITERATOR_DEBUG_LEVEL=0")
# else ()
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -I /usr/local/include -I ../../include -DCURL_STATICLIB -Bstatic")
# endif ()
# find_package(jsoncpp REQUIRED)
# find_package(fmt REQUIRED)
list(APPEND LIBRARIES fmt jsoncpp_static)

if (USE_KEPLER_STATIC)
  # list(APPEND fmt_static jsoncpp_static)
  add_library(Kepler STATIC
    ${SOURCE_FILES}
  )
else()
  # list(APPEND fmt_shared jsoncpp_shared)
  add_library(Kepler SHARED
    ${SOURCE_FILES}
  )
endif()
# if (BUILD_SHARED_LIBS)
#     add_library(Kepler SHARED
#       ${HTTP_SERVER_SOURCE_FILES}
#       # src/main.cpp
#     )
# else()

target_link_libraries(Kepler
  ${LIBRARIES}
)

# endif()
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set_property(TARGET Kepler PROPERTY POSITION_INDEPENDENT_CODE ON)
# target_link_libraries(Kepler
#     ${Boost_LIBRARIES}
#     Logger
#     fmt
#   )
# target_compile_features(Kepler PRIVATE cxx_std_20)

# set_target_properties(Kepler_static PROPERTIES
#   LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD"
# )
# set_target_properties(Kepler_static PROPERTIES
#   RuntimeLibrary MultiThreadedDebugDLL
# )
# target_compile_features(Kepler_static PRIVATE cxx_std_20)
# find_package(OpenSSL REQUIRED)
# OpenSSL::SSL
# OpenSSL::Crypto

# Build examples
# if (KEPLER_BUILD_EXAMPLES)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
# endif()

add_executable(MultithreadHttpServer examples/MultithreadHttpServer/src/main.cpp)
target_link_libraries(MultithreadHttpServer jsoncpp_static Kepler)