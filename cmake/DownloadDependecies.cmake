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

# cppcoro
FetchContent_Declare(cppcoro
  GIT_REPOSITORY https://github.com/DeepForge-Tech/cppcoro.git
  GIT_TAG main
  GIT_SHALLOW 1
)
# if(NOT fmt_POPULATED)
#   FetchContent_Populate(fmt)

#   add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(cppcoro)

 # jsoncpp
 FetchContent_Declare(jsoncpp
 GIT_REPOSITORY https://github.com/DeepForge-Technology/jsoncpp.git
 GIT_TAG master
)
# if(NOT jsoncpp_POPULATED)
#   FetchContent_Populate(jsoncpp)
#   add_subdirectory(${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR})
# endif()
FetchContent_MakeAvailable(jsoncpp)