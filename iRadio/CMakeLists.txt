cmake_minimum_required(VERSION 3.5)

# 定义目标名称为testWindow
add_executable(testWindow main.cpp)

# 链接需要的库，比如Boost和OpenCV
target_link_libraries(testWindow
        PRIVATE
        MainWindow  # 这是你在 gui 目录中定义的动态库
        opencv_world
        SA::SARibbonBar  # 链接SARibbonBar库
        iRadio_python   # python文件夹下编译出来的库
)

# 指定包含路径
target_include_directories(testWindow
        PRIVATE
        ${OpenCV_INCLUDE_DIRS}
#        ${CMAKE_SOURCE_DIR}/gui  # 添加 gui 目录的包含路径
        ${CMAKE_SOURCE_DIR}/python  # 添加 gui 目录的包含路径
)
#########################################################################
# 定义一个自定义命令，将 GUI 动态库和 SARibbonBar 的 DLL 拷贝到可执行文件目录
add_custom_command(TARGET testWindow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:MainWindow>
        $<TARGET_FILE:SARibbonBar>
        $<TARGET_FILE:iRadio_python>
        $<TARGET_FILE_DIR:testWindow>
        COMMENT "Copying GUI and SARibbonBar DLLs to the executable directory."
)

# 将 OpenCV World DLL 文件复制到可执行文件目录
if(OpenCV_FOUND)
    add_custom_command(TARGET testWindow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OpenCV_DIR}/bin/libopencv_world4100.dll"  # 替换为你的 OpenCV World DLL 路径
            $<TARGET_FILE_DIR:testWindow>
            COMMENT "Copying OpenCV World DLL to the executable directory."
    )
endif()

# 复制 Python DLL 文件
add_custom_command(TARGET testWindow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SOURCE_LIB_DIR}/python/python312.dll"  # 替换为你的 Python DLL 文件路径
        $<TARGET_FILE_DIR:testWindow>
        COMMENT "Copying Python DLL to the executable directory."
)