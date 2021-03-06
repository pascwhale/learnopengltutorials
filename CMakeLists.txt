cmake_minimum_required(VERSION 3.11)

project(Clean)

include(ExternalProject)

# Set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)
if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug;Release" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)

# Clean
set(EXEC Clean)

file(GLOB SRC src/*.cpp)

add_executable(${EXEC} ${SRC})

target_link_libraries(${EXEC} OpenGL::GL glew_s glfw glm)

list(APPEND BIN ${EXEC})
# end Clean

# install files to install location
install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})

# copt folders to build directory
file(COPY src/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY src/textures DESTINATION ${CMAKE_BINARY_DIR})
