cmake_minimum_required(VERSION 3.27)
project(OcicatRecoilEditor CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")

find_package(OpenGL REQUIRED)

set(imgui_src
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui_demo.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui_tables.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui_widgets.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/misc/cpp/imgui_stdlib.cpp"
)
set(imgui_headers
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imconfig.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imgui_internal.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imstb_rectpack.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imstb_textedit.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/imstb_truetype.h"
	"${CMAKE_CURRENT_LIST_DIR}/lib/imgui/misc/cpp/imgui_stdlib.h"
)

set(ocicat_src
	"src/main.cpp"
	"src/editor.cpp"
	"src/pattern.cpp"
	"lib/imgui-sfml-2.6/imgui-SFML.cpp"
	${imgui_src}
)

set(ocicat_include
	"src"
	"lib"
	"lib/imgui"
	"lib/imgui/backends"
	"lib/imgui-sfml-2.6"
	"lib/SFML-2.6.1/include"
)

set(ocicat_link_dirs
	"lib/SFML-2.6.1/lib"
)

set(ocicat_libs
	freetype
	sfml-graphics-d
	sfml-main-d
	sfml-system-d
	sfml-window-d
	${OPENGL_LIBRARIES}
)

add_executable(OcicatRecoilEditor)
target_sources(OcicatRecoilEditor PUBLIC ${ocicat_src})
target_include_directories(OcicatRecoilEditor PUBLIC ${ocicat_include})
target_link_libraries(OcicatRecoilEditor PUBLIC ${ocicat_libs})
target_link_directories(OcicatRecoilEditor PUBLIC ${ocicat_link_dirs})