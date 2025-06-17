cmake_minimum_required(VERSION 3.31)
project(Tetris)
set(CMAKE_CXX_STANDARD 26)
include_directories(D:/CLion/mingw64/include)
find_package(SFML 3.0.0 REQUIRED COMPONENTS System Window Graphics Audio Network) # 检查你的 SFML 版本
#set(SFML_STATIC_LIBRARIES TRUE)

# 设置 SFML 的路径（根据你的安装位置修改）
set(SFML_DIR "D:/CLion/mingw64/lib/cmake/SFML/")
# 对于 Windows，可能是 "C:/SFML-2.x/lib/cmake/SFML"

# 添加可执行文件并链接 SFML 库
add_executable(Tetris main.cpp
        C_RandomNum/RandomNum.cpp
        C_RandomNum/RandomNum.h)
target_link_libraries(Tetris PRIVATE SFML::Graphics SFML::Audio SFML::Network SFML::System SFML::Window)