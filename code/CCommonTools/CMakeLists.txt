cmake_minimum_required(VERSION 3.16)
project(CCommonTools LANGUAGES CXX)

# 设置C++标准和Qt自动处理
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${PROJECT_NAME} SHARED
    CommonTools.h
    Config.h
    DatabaseTool.h
    LongTextEdit.h
    StringHelper.h
    TextEditWidget.h
    PathListDialog.h
    TreeItem.h
    ccommontools_global.h
    tableitemmodel.h
    DialogBase.h
    treeitemmodel.h
    CommonTools.cpp
    Config.cpp
    DatabaseTool.cpp
    LongTextEdit.cpp
    StringHelper.cpp
    TextEditWidget.cpp
    PathListDialog.cpp
    TreeItem.cpp
    tableitemmodel.cpp
    DialogBase.cpp
    treeitemmodel.cpp
)

# 依赖的Qt组件
set(QT_COMPONENTS Core Sql Widgets)
# 尝试先找到Qt6，如果找不到再尝试Qt5
find_package(Qt6 COMPONENTS ${QT_COMPONENTS} QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 5 COMPONENTS ${QT_COMPONENTS} REQUIRED)
    set(QT_VERSION_MAJOR 5)
endif()

# 将Qt组件转换为链接目标，根据Qt版本自动添加前缀
list(TRANSFORM QT_COMPONENTS PREPEND "Qt${QT_VERSION_MAJOR}::" OUTPUT_VARIABLE QT_LINK_TARGETS)

# 包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# 编译宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE CCOMMONTOOLS_LIB)

# 链接目录与库
target_link_directories(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../../bin/debug>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../../bin/release>
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LINK_TARGETS}
)
