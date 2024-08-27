cmake_minimum_required(VERSION 3.18)
cmake_policy(SET CMP0144 NEW)
option(USE_KEPLER_STATIC "Build static library" OFF)
option(USE_RUNTIME_STATIC "Build static runtime" OFF)
option(USE_HTTP "Use libraries for HTTP" ON)
option(KEPLER_BUILD_EXAMPLES "Build examples" OFF)
option(USE_WEBCLIENT "Use libraries for http/https web client" ON)

# set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if (USE_KEPLER_STATIC)
  if (WIN32)
    set(BOOST_ROOT "C:\\Libraries\\lib\\boost-static")
  elseif(UNIX)
    set(BOOST_ROOT "/usr/local/Libraries/lib/boost-static")
  elseif(APPLE)
  endif()
  set(Boost_USE_STATIC_LIBS ON)
  set(BOOST_ALL_DYN_LINK OFF)
  set(BUILD_STATIC_LIBS ON)
  set(BUILD_SHARED_LIBS OFF)
  set(BUILD_STATIC_LIBS ON)
else()
  if (WIN32)
    set(BOOST_ROOT "C:\\Libraries\\lib\\boost-shared")
  elseif(UNIX)
    set(BOOST_ROOT "/usr/local/Libraries/lib/boost-shared")
  elseif(APPLE)
  endif()
  set(BUILD_STATIC_LIBS OFF)
  set(Boost_USE_STATIC_LIBS OFF)
  set(BOOST_ALL_DYN_LINK ON)
  set(BUILD_SHARED_LIBS ON)
  set(BUILD_STATIC_LIBS OFF)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

if (USE_RUNTIME_STATIC)
  if (MSVC)
    add_compile_options(
      $<$<CONFIG:>:/MT> #---------|
      $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
      $<$<CONFIG:Release>:/MT> #--|
    )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
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
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a .lib")
    elseif(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    elseif(APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-shared")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    if (WIN32)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
    elseif(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    elseif(APPLE)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "-shared -undefined dynamic_lookup")
  endif()
  set(BOOST_RUNTIME_LINK "shared")
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

project(Kepler)

add_definitions(-D_WIN32_WINNT=0x0601)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadDependecies.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# find_package(PkgConfig REQUIRED)

# Source files
set(HTTP_SERVER_SOURCE_FILES)
set(WEBCLIENT_SOURCE_FILES)
set(SOURCE_FILES)

# Libraries
set(HTTP_SERVER_LIBRARIES)
set(WEBCLIENT_LIBRARIES)
set(LIBRARIES)
# set(BOOST_INCLUDE_LIBRARIES system coroutine context asio beast)
# set(BOOST_ENABLE_CMAKE ON)

# include(FetchContent)
# FetchContent_Declare(
#   Boost
#   GIT_REPOSITORY https://github.com/boostorg/boost.git
#   GIT_PROGRESS TRUE
#   GIT_TAG boost-1.84.0
#   GIT_SHALLOW 1
# )
# FetchContent_MakeAvailable(Boost)

# include_directories(${BOOST_LIBRARY_INCLUDES})
# include_directories(asio INTERFACE ${boost_asio_SOURCE_DIR}/include)
# include_directories(coroutine INTERFACE ${boost_coroutine_SOURCE_DIR}/include)
# include_directories(system INTERFACE ${boost_system_SOURCE_DIR}/include)
# include_directories(context INTERFACE ${boost_context_SOURCE_DIR}/include)
# include_directories(beast INTERFACE ${boost_beast_SOURCE_DIR}/include)

CPMAddPackage(
  NAME         fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION      11.0.2
  CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
)
# Add cppcoro dependency
CPMAddPackage(
  NAME         cppcoro
  GITHUB_REPOSITORY DeepForge-Tech/cppcoro
  VERSION      1.0.0
  CMAKE_ARGS -DBUILD_TESTING=OFF
)
# Add jsoncpp dependency
CPMAddPackage(
  NAME         jsoncpp
  GITHUB_REPOSITORY DeepForge-Tech/jsoncpp
  VERSION      1.9.5
  CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
)

message("Use libraries for HTTP - ${USE_HTTP}")
if (USE_HTTP)
  find_package(Boost REQUIRED COMPONENTS system coroutine context asio beast)

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
  list(APPEND SOURCE_FILES ${HTTP_SERVER_SOURCE_FILES})
  list(APPEND HTTP_SERVER_LIBRARIES  Boost::asio Boost::system Boost::context Boost::coroutine Boost::beast Logger)
  list(APPEND LIBRARIES ${HTTP_SERVER_LIBRARIES})
endif()

if (USE_WEBCLIENT)
  # find_package(OpenSSL REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system coroutine context asio beast)
  include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND WEBCLIENT_SOURCE_FILES src/WebClient/WebClient.cpp src/HttpClient/HttpClient.cpp src/HttpsClient/HttpsClient.cpp)
  list(APPEND WEBCLIENT_LIBRARIES Boost::asio Boost::system Boost::context Boost::coroutine Boost::beast)
  if (WIN32)
    CPMAddPackage(
      NAME         openssl
      GITHUB_REPOSITORY DeepForge-Tech/openssl-cmake
      VERSION      1.1
      CMAKE_ARGS -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    )
    list(APPEND WEBCLIENT_LIBRARIES ssl crypto)
  else()
    find_package(OpenSSL REQUIRED)
    list(APPEND WEBCLIENT_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
  endif()
  if (USE_KEPLER_STATIC)
    list(APPEND WEBCLIENT_LIBRARIES cppcoro)
  else()
    list(APPEND WEBCLIENT_LIBRARIES cppcoro_shared)
  endif()
  list(APPEND SOURCE_FILES ${WEBCLIENT_SOURCE_FILES})
  list(APPEND LIBRARIES ${WEBCLIENT_LIBRARIES})
endif()

list(APPEND LIBRARIES fmt jsoncpp_lib)
if (USE_KEPLER_STATIC)
  add_library(Kepler STATIC
    ${SOURCE_FILES}
  )
  target_link_libraries(Kepler
    ${LIBRARIES}
  )
else()
  
  add_library(Kepler SHARED
    ${SOURCE_FILES}
  )
  include(GenerateExportHeader)
  generate_export_header(Kepler)
  target_link_libraries(Kepler
    ${LIBRARIES}
  )
endif()



# Build examples
# if (KEPLER_BUILD_EXAMPLES)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
# endif()

add_executable(MultithreadHttpServer examples/MultithreadHttpServer/src/main.cpp)
target_link_libraries(MultithreadHttpServer jsoncpp_lib Kepler)