cmake_minimum_required(VERSION 3.12)
project(myProject)

set(CMAKE_CXX_STANDARD 17)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# -------------------------------------------------------
# platform dependent backend and files
# -------------------------------------------------------

if (WIN32)
    message(STATUS "Building for Windows")

    set(IMGUI_BACKENDS
        ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )

    set(APP_MAIN src/main_windows.cpp)

elseif(APPLE)
    message(STATUS "Building for macOS")

    set(IMGUI_BACKENDS
        ${IMGUI_DIR}/backends/imgui_impl_osx.mm
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )

    set(APP_MAIN src/main_mac.cpp)

endif()

# ImGui
add_library(imgui STATIC
    ${IMGUI_SRC}
    ${IMGUI_BACKENDS}
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# -------------------------------------------------------
# App
# -------------------------------------------------------
add_executable(myApp
    ${APP_MAIN}
    src/app.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(myApp PRIVATE
    imgui
    OpenGL::GL
)

# Windows: dwmapi
if (WIN32)
	target_link_libraries(myApp PRIVATE 
		dwmapi
	)
endif()

# macOS: Cocoa, IOKit, CoreVideo
if (APPLE)
    target_link_libraries(myApp
        PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
