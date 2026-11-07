cmake_minimum_required(VERSION 3.24)
project(doom_rl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
  GIT_SHALLOW TRUE
)
set(CLI11_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  CLI11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.7.2
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(vizdoom yaml-cpp CLI11)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio)

add_executable(doom_rl
  src/main.cpp
  src/config/Config.cpp
  src/util/Utils.cpp
  src/env/ViZDoomEnv.cpp
  src/agents/RandomAgent.cpp
  src/agents/AgentRegistry.cpp
  src/app/Cli.cpp
  src/run/Runner.cpp
)
target_include_directories(doom_rl PRIVATE include ${vizdoom_SOURCE_DIR}/include)
target_compile_definitions(doom_rl PRIVATE
  VIZDOOM_ENGINE_PATH="${CMAKE_BINARY_DIR}/bin/vizdoom"
  VIZDOOM_IWAD_PATH="${vizdoom_SOURCE_DIR}/src/freedoom2.wad"
  VIZDOOM_SCENARIOS_DIR="${vizdoom_SOURCE_DIR}/scenarios"
)
target_link_libraries(doom_rl PRIVATE libvizdoom_static yaml-cpp::yaml-cpp CLI11::CLI11 ${OpenCV_LIBS})
