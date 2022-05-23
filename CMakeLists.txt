cmake_minimum_required(VERSION 3.21)
project(openGL)

set(CMAKE_CXX_STANDARD 14)

# 添加头文件
set(HEADERS /usr/local/Cellar)
set(HEADERS /usr/local/include)
include_directories(${HEADERS})

set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.6/include/GLFW)
set(FREE_H /usr/local/Cellar/freeglut/3.2.1_1/include/GL)
set(GLAD_H /usr/local/include/glad)
set(KH_H /usr/local/include/KHR)
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${FREE_H})


set(GLFW_LINK /usr/local/Cellar/glfw/3.3.6/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

set(ASSIMP /usr/local/Cellar/assimp/5.1.2/lib/libassimp.5.1.0.dylib)
link_libraries(${ASSIMP})


# 执行编译命令

add_executable(Model ModelLoad/openGLFW.cpp glad.c
        ModelLoad/Camera.cpp ModelLoad/ffAnimation.cpp
        ModelLoad/ffImage.cpp ModelLoad/ffMesh.cpp ModelLoad/ffModel.cpp
        ModelLoad/Shader.cpp ModelLoad/ffBone.cpp ModelLoad/ffAnimator.cpp)

find_package(assimp REQUIRED) # 添加assimp


if (APPLE)
    target_link_libraries(Model "-framework OpenGL")
    target_link_libraries(Model "-framework GLUT")
    target_link_libraries(Model assimp::assimp)

endif()







