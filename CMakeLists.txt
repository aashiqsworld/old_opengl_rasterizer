cmake_minimum_required(VERSION 3.26)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.8/include/GLFW)
set(GLAD_H ${CMAKE_CURRENT_SOURCE_DIR}/include/glad)
set(KHR_H ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR)
set(GLM_H ${CMAKE_CURRENT_SOURCE_DIR}/include/glm)
include_directories(${KHR_H} ${GLAD_H} ${GLFW_H} ${GLM_H})


set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLFW_LINK})

set(SOURCE_FILES main.cpp)
add_executable(learn_opengl main.cpp
        shader.h
        stb_image.h
        stb_image.cpp
        camera.h)

if (APPLE)
    target_link_libraries(learn_opengl "-framework OpenGL")
    target_link_libraries(learn_opengl "-framework GLUT")
endif()