cmake_minimum_required(VERSION 3.22)
project(oop_project)  # 项目名称 oop_project

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(SFML 2.5 COMPONENTS
        system window graphics audio REQUIRED)  # 依赖包 SFML 库版本 2.5 及其所需的组件

file(GLOB_RECURSE SOURCES "src/*.cpp")  # 搜索 src 目录下的所有 cpp 文件
add_executable(main ${SOURCES} src/Game.cpp)  # 生成可执行文件 main
target_include_directories(main PUBLIC src thirdparty/SFML/include)  # 添加头文件搜索路径
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)  # 添加链接库搜索路径
set_property(TARGET main PROPERTY
        CXX_STANDARD          11
        CXX_STANDARD_REQUIRED ON)  # 设置 C++ 标准为 C++11

file(COPY "assets" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")  # 复制 assets 目录到可执行文件所在目录
