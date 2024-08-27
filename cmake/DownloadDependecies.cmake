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