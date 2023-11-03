cmake_minimum_required(VERSION 3.24)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        Sql
        REQUIRED)

add_executable(server main.cpp src/TCPServer.cpp src/TCPServer.h src/MainWindow.cpp src/MainWindow.h src/ConnectionInfo.h src/DatabaseManager.h src/DatabaseManager.cpp)
target_link_libraries(server
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Sql
        )

