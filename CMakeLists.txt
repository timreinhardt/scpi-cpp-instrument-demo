cmake_minimum_required(VERSION 3.16)

project(scpi_cpp_instrument_demo LANGUAGES CXX)

# =====================================================
# C++ Standard
# =====================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================================================
# CLI Executable
# =====================================================

add_executable(scpi_cpp_demo
    ui/main.cpp
    core/src/ScpiClient.cpp
    core/src/TraceData.cpp
    transport/src/TcpTransport.cpp
)

target_include_directories(scpi_cpp_demo PRIVATE
    core/include
    transport/include
)

# =====================================================
# Qt6
# =====================================================

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# =====================================================
# Qt GUI Executable
# =====================================================

add_executable(scpi_cpp_qt_demo
    qt-ui/main_qt.cpp
    qt-ui/MainWindow.cpp
    qt-ui/TraceWidget.cpp
    core/src/ScpiClient.cpp
    core/src/TraceData.cpp
    transport/src/TcpTransport.cpp
)

target_include_directories(scpi_cpp_qt_demo PRIVATE
    core/include
    transport/include
    qt-ui
)

target_link_libraries(scpi_cpp_qt_demo PRIVATE
    Qt6::Widgets
)