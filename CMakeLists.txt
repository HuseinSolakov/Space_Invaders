cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#project
project(SpaceInvaders_Like VERSION 0.07)

#opengl
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

#Executable vars
file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE C_FILES src/*.c)

#Executables
add_executable(${PROJECT_NAME} main.cpp ${CPP_FILES} ${C_FILES})

#Header files
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/ImGui ${CMAKE_CURRENT_SOURCE_DIR}/include/glad)

#GLFW
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS})
