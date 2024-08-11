cmake_minimum_required(VERSION 3.18)
if (WIN32)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
# set(VCPKG_CRT_LINKAGE static)
# set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "VCPKG Target Triplet to use")
endif()
# set(BOOST_ROOT "C:/Boost/")

if (MSVC)
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedStatic")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()
project(Kepler)

add_definitions(-D_WIN32_WINNT=0x0601)
# cmake_policy(SET CMP0144 NEW)
# cmake_policy(SET CMP0135 NEW)
set(CMAKE_CXX_STANDARD 20)
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

message("Use libraries for HTTP - ${USE_HTTP}")
if (USE_HTTP)
  
  set (Boost_NO_SYSTEM_PATHS ON)
  set (Boost_USE_MULTITHREADED ON)
  set (Boost_USE_STATIC_LIBS ON)
  set (BOOST_ALL_DYN_LINK OFF)
  # set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
  # set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
  # set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
  # set(Boost_USE_MULTITHREADED      ON)
  # set(Boost_USE_STATIC_RUNTIME     ON)
  find_package(Boost REQUIRED COMPONENTS system coroutine context)

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
  list(APPEND HTTP_SERVER_LIBRARIES  ${Boost_LIBRARIES} Logger)
  list(APPEND LIBRARIES ${HTTP_SERVER_LIBRARIES})
endif()

if (USE_WEBCLIENT)
  set (Boost_NO_SYSTEM_PATHS ON)
  set (Boost_USE_MULTITHREADED ON)
  set (Boost_USE_STATIC_LIBS ON)
  set (BOOST_ALL_DYN_LINK OFF)
  find_package(OpenSSL REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system coroutine context)
  list(APPEND WEBCLIENT_SOURCE_FILES src/WebClient/WebClient.cpp src/HttpClient/HttpClient.cpp src/HttpsClient/HttpsClient.cpp)
  list(APPEND LIBRARIES ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto cppcoro)
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

list(APPEND LIBRARIES fmt jsoncpp_static)

# if (BUILD_SHARED_LIBS)
#     add_library(Kepler SHARED
#       ${HTTP_SERVER_SOURCE_FILES}
#       # src/main.cpp
#     )
# else()
# add_library(Kepler SHARED
#   ${HTTP_SERVER_SOURCE_FILES}
# )

add_library(Kepler_static STATIC
  ${SOURCE_FILES}
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
target_link_libraries(Kepler_static 
  ${LIBRARIES}
)
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
target_link_libraries(MultithreadHttpServer jsoncpp_static Kepler_static)