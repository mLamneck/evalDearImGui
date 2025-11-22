
cmake_minimum_required(VERSION 3.12)
project(myProject)

set(CMAKE_CXX_STANDARD 17)

# ----------------------------------------
# Fetch GLFW (cross-platform windowing)
# ----------------------------------------
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# ----------------------------------------
# IMGUI
# ----------------------------------------
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

set(IMGUI_BACKENDS
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC
    ${IMGUI_SRC}
    ${IMGUI_BACKENDS}
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

# ----------------------------------------
# Your executable
# ----------------------------------------
add_executable(myApp 
    src/main.cpp
	src/app.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(myApp PRIVATE
    imgui
    glfw
    OpenGL::GL
)

# ----------------------------------------
# macOS frameworks for GLFW/OpenGL
# ----------------------------------------
if(APPLE)
    target_link_libraries(myApp PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
