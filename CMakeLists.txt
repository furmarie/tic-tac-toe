cmake_minimum_required(VERSION 3.7)
project(Tic-Tac-Toe)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION}) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    if(EMSCRIPTEN)
        set(PLATFORM Web CACHE STRING "" FORCE)
    endif()
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()


file(GLOB sources src/*.cc)

add_executable(ttt ${sources})
target_link_libraries(ttt raylib)

if(EMSCRIPTEN) 
  target_compile_definitions(ttt PUBLIC -DPLATFORM=Web)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
endif()
