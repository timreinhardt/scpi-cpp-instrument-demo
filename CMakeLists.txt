cmake_minimum_required(VERSION 3.16)

project(scpi_cpp_instrument_demo LANGUAGES CXX)

# =====================================================
# C++ / CMake Settings
# =====================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for VS Code / IntelliSense.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =====================================================
# CLI Executable
# =====================================================
#
# Simple command-line SCPI demo.
# Does not use Qt.
#

add_executable(scpi_cpp_demo
    ui/main.cpp

    core/src/ScpiClient.cpp
    core/src/TraceData.cpp
    core/src/FieldGrid.cpp

    transport/src/TcpTransport.cpp
)

target_include_directories(scpi_cpp_demo PRIVATE
    core/include
    transport/include
)

# =====================================================
# Qt6 Modules
# =====================================================
#
# Widgets:
#   Standard Qt desktop UI controls.
#
# DataVisualization:
#   3D surface plotting via Q3DSurface.
#

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    DataVisualization
)

# =====================================================
# Qt GUI Executable
# =====================================================
#
# Full desktop GUI:
# - SCPI command controls
# - 1D trace graph
# - 2D heatmap
# - 3D surface view
#

add_executable(scpi_cpp_qt_demo
    qt-ui/main_qt.cpp
    qt-ui/MainWindow.cpp
    qt-ui/TraceWidget.cpp
    qt-ui/HeatmapWidget.cpp
    qt-ui/Surface3DWidget.cpp

    core/src/ScpiClient.cpp
    core/src/TraceData.cpp
    core/src/FieldGrid.cpp

    transport/src/TcpTransport.cpp
)

target_include_directories(scpi_cpp_qt_demo PRIVATE
    core/include
    transport/include
    qt-ui
)

target_link_libraries(scpi_cpp_qt_demo PRIVATE
    Qt6::Widgets
    Qt6::DataVisualization
)
# =====================================================
# Platform-specific socket libraries
# =====================================================
#
# Windows sockets require linking against ws2_32.
# macOS/Linux do not need this extra library.
#
if(WIN32)
    target_link_libraries(scpi_cpp_demo PRIVATE ws2_32)
    target_link_libraries(scpi_cpp_qt_demo PRIVATE ws2_32)
endif()