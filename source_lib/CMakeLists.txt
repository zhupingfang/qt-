# 设置库的头文件路径
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# 添加静态库或动态库
#add_library(yourlib STATIC IMPORTED)
#set_target_properties(yourlib PROPERTIES
#        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/src/yourlib.lib
#)
#
## 添加 DLL 文件到构建目录
#add_custom_command(TARGET yourlib POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${CMAKE_SOURCE_DIR}/src/src/yourlib.dll
#        $<TARGET_FILE_DIR:yourlib>
#)

add_subdirectory(SARibbonBar)