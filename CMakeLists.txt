CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(CPP-LRU_CACHE)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Build examples by default:
OPTION(BUILD_EXAMPLES "Build cpp-lru-cache examples." ON)

IF(BUILD_EXAMPLES)
    INCLUDE_DIRECTORIES(
	   ${PROJECT_SOURCE_DIR}/include)

    LINK_DIRECTORIES()

    ADD_EXECUTABLE(cpp-lru-cache-test
        src/test)

    TARGET_LINK_LIBRARIES(cpp-lru-cache-test)

    SET_TARGET_PROPERTIES(cpp-lru-cache-test PROPERTIES
        PREFIX ""
        SUFFIX ""
        COMPILE_FLAGS "-std=c++0x -W -Wall -pedantic")

    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CSS_FLAGS_RELEASE "-03 -g")
ENDIF(BUILD_EXAMPLES)

# Add an interface library if cmake is version 3.1 or newer
IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
  ADD_LIBRARY(cpp-lru-cache INTERFACE)
  TARGET_INCLUDE_DIRECTORIES(cpp-lru-cache INTERFACE ${PROJECT_SOURCE_DIR}/include)
  TARGET_SOURCES(cpp-lru-cache INTERFACE ${PROJECT_SOURCE_DIR}/include/lrucache.hpp)
ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
