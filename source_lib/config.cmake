# src/core/YourLibConfig.cmake

# 设置库的路径
set(YourLib_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
set(YourLib_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/src/yourlib.lib")

# 目标配置
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YourLib DEFAULT_MSG YourLib_INCLUDE_DIRS YourLib_LIBRARIES)

# 声明库
set(YourLib_FOUND TRUE)



#######################################
# src/core/CMakeLists.txt

# 设置库的头文件路径
include_directories(${CMAKE_SOURCE_DIR}/src/core/include)

# 添加库
add_library(YourLib STATIC IMPORTED)
set_target_properties(YourLib PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/core/src/yourlib.lib
)

# 安装CMake配置文件
install(FILES ${CMAKE_SOURCE_DIR}/src/core/YourLibConfig.cmake
        DESTINATION lib/cmake/YourLib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/include/
        DESTINATION include)



############主要cmake
# 主CMakeLists.txt

include_directories(${CMAKE_SOURCE_DIR}/src/core/include)

# 添加子目录
add_subdirectory(src/core)

# 安装项目
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/include/
        DESTINATION include)


#####################################################
#使用find_package在其他项目中
#在需要使用这个库的项目的CMakeLists.txt中，使用find_package来查找和配置库：
# gui/CMakeLists.txt

# 查找自定义库
find_package(YourLib REQUIRED)

# 添加可执行文件
add_executable(MyApp main.cpp)

# 链接库
target_link_libraries(MyApp YourLib::YourLib)
