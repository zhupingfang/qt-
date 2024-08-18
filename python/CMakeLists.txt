cmake_minimum_required(VERSION 3.12)

set(IRADIO_PYTHON_LIB iRadio_python)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#########################################################################
# 设置Python库的包含路径
include_directories("${SOURCE_LIB_DIR}/python/include")

# 设置Python库的链接路径
link_directories("${SOURCE_LIB_DIR}/python/lib")

# 查找 Python 解释器、开发环境和库
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# 添加包含目录
include_directories(${Python_INCLUDE_DIRS})

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
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

#########################################################################
# 编译成一个库
add_library(${IRADIO_PYTHON_LIB} SHARED ${SOURCE_FILES})

#########################################################################
# 设置包含目录和链接库为PUBLIC，使得链接到这个库的其他目标也能继承这些设置

target_link_libraries(${IRADIO_PYTHON_LIB} PUBLIC
        ${Python_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(${IRADIO_PYTHON_LIB} PUBLIC ${Python_INCLUDE_DIRS} ${INCLUDE_DIRS})