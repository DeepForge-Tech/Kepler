add_definitions(-D_WIN32_WINNT=0x0601)
cmake_policy(SET CMP0144 NEW)
# cmake_policy(SET CMP0135 NEW)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(USE_HTTP "Use libraries for HTTP" ON)
option(KEPLER_BUILD_EXAMPLES "Build examples" OFF)
option(USE_WEBCLIENT "Use libraries for http/https web client" ON)

include(../DownloadDependecies.cmake)
include_directories(../../include)

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
  
#   set (Boost_NO_SYSTEM_PATHS ON)
#   set (Boost_USE_MULTITHREADED ON)
#   set (Boost_USE_STATIC_LIBS ON)
#   set (BOOST_ALL_DYN_LINK OFF)
#   # set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
#   # set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
#   # set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
#   # set(Boost_USE_MULTITHREADED      ON)
#   # set(Boost_USE_STATIC_RUNTIME     ON)
#   find_package(Boost REQUIRED COMPONENTS system coroutine context)

#   include_directories(${Boost_INCLUDE_DIRS})
  list(APPEND HTTP_SERVER_SOURCE_FILES
    ../../src/HttpServer/DefaultMiddlewares.cpp
    ../../src/HttpServer/HttpRequest.cpp
    ../../src/HttpServer/HttpResponse.cpp
    ../../src/HttpServer/HttpServer.cpp
    ../../src/HttpServer/Middleware.cpp
    ../../src/HttpServer/Router.cpp
    ../../src/HttpServer/Session.cpp
  )
  list(APPEND SOURCE_FILES ${HTTP_SERVER_SOURCE_FILES})
#   list(APPEND HTTP_SERVER_LIBRARIES  ${Boost_LIBRARIES} Logger)
#   list(APPEND LIBRARIES ${HTTP_SERVER_LIBRARIES})
endif()

if (USE_WEBCLIENT)
#   set (Boost_NO_SYSTEM_PATHS ON)
#   set (Boost_USE_MULTITHREADED ON)
#   set (Boost_USE_STATIC_LIBS ON)
#   set (BOOST_ALL_DYN_LINK OFF)
#   find_package(OpenSSL REQUIRED)
#   find_package(Boost REQUIRED COMPONENTS system coroutine context)
    list(APPEND WEBCLIENT_SOURCE_FILES 
        ../../src/WebClient/WebClient.cpp 
        ../../src/HttpClient/HttpClient.cpp 
        ../../src/HttpsClient/HttpsClient.cpp
    )
#   list(APPEND LIBRARIES ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto cppcoro)
  list(APPEND SOURCE_FILES ${WEBCLIENT_SOURCE_FILES})
endif()