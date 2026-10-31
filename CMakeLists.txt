cmake_minimum_required(VERSION 3.24)
project(doom_rl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
set(BUILD_PYTHON OFF CACHE BOOL "" FORCE)
set(BUILD_ENGINE ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  vizdoom
  GIT_REPOSITORY https://github.com/Farama-Foundation/ViZDoom.git
  GIT_TAG 1.3.2
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(vizdoom)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio)

add_executable(doom_rl src/main.cpp)
target_include_directories(doom_rl PRIVATE ${vizdoom_SOURCE_DIR}/include)
target_link_libraries(doom_rl PRIVATE libvizdoom_static ${OpenCV_LIBS})
