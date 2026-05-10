cmake_minimum_required(VERSION 3.16)
project(AddCodeHeader LANGUAGES CXX)

# 设置C++标准和Qt自动处理
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${PROJECT_NAME} SHARED
        AddCodeHeader.h
        AddCodeHeaderEditor.h
        AddFileHeadThread.h
        CRemoveCommentThread.h
        CommonData.h
        FileHeadHelper.h
        HeadItemTableModel.h
        MainWidget.h
        RemoveCommentHelper.h
        Test.h
        addcodeheader_global.h
        AddCodeHeader.cpp
        AddCodeHeaderEditor.cpp
        AddFileHeadThread.cpp
        CRemoveCommentThread.cpp
        CommonData.cpp
        FileHeadHelper.cpp
        HeadItemTableModel.cpp
        MainWidget.cpp
        RemoveCommentHelper.cpp
        Test.cpp
)

# 依赖的Qt组件
set(QT_COMPONENTS Core Gui Widgets)
# 尝试先找到Qt6，如果找不到再尝试Qt5
find_package(Qt6 COMPONENTS ${QT_COMPONENTS} QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 5.15 COMPONENTS ${QT_COMPONENTS} REQUIRED)
    set(QT_VERSION_MAJOR 5)
endif()

# 将Qt组件转换为链接目标，根据Qt版本自动添加前缀
list(TRANSFORM QT_COMPONENTS PREPEND "Qt${QT_VERSION_MAJOR}::" OUTPUT_VARIABLE QT_LINK_TARGETS)

# 包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# 编译宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE ADDCODEHEADER_LIB)

# 输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    # 可执行程序输出路径
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/debug
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/release
)

# 链接目录与库
target_link_directories(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../../bin/debug>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../../bin/release>
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    CCommonTools
    ${QT_LINK_TARGETS}
)
