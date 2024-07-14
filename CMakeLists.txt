cmake_minimum_required(VERSION 3.18)
project(Kepler)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
# Fetch nghttp2-asio
# FetchContent_Declare(
#   nghttp2_asio
#   GIT_REPOSITORY https://github.com/nghttp2/nghttp2-asio.git
#   GIT_TAG main  # Вы можете изменить версию, если требуется
# )
# FetchContent_MakeAvailable(nghttp2_asio)

find_package(Boost REQUIRED COMPONENTS system json)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Kepler src/main.cpp)
target_link_libraries(Kepler 
  ${Boost_LIBRARIES}
  OpenSSL::SSL
  OpenSSL::Crypto
  # nghttp2_asio
)
