cmake_minimum_required(VERSION 3.5)

#project(MainWindow LANGUAGES CXX)
set(GUI_LIB MainWindow)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#########################################################################
# 包含资源文件
set(RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/saribbonresouce.qrc)

#########################################################################
# 递归获取当前目录及其子目录中的所有 .h 文件
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(INCLUDE_DIRS)
foreach(HEADER_FILE ${HEADER_FILES})
    get_filename_component(DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${DIR})
endforeach()

# 去除重复的路径
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# 输出包含路径（可选，用于调试）
message(STATUS "Include directories: ${INCLUDE_DIRS}")

# 设置包含目录
include_directories(${INCLUDE_DIRS})

#########################################################################
# 递归获取当前目录及其子目录中的所有 .cpp 文件
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

############################# 创建动态库 ######################################
add_library(${GUI_LIB} SHARED ${SOURCE_FILES} ${RESOURCE_FILES})

#########################################################################
# 针对 Windows 平台的特殊设置
if (WIN32)
    target_compile_definitions(${GUI_LIB} PRIVATE BUILD_DATA_DEFINE_DLL)
endif()

# 设置库的包含目录和链接库
target_include_directories(${GUI_LIB} PRIVATE
        ${INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/source_lib/SARibbonBar  # 假设SARibbonBar头文件所在目录
)

target_link_libraries(${GUI_LIB} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        SA::SARibbonBar
)
