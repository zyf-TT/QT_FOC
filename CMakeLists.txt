cmake_minimum_required(VERSION 3.16)

project(FOC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

# 源文件列表
set(PROJECT_SOURCES
    main.cpp
    # UI层
    src/ui/MainWindow.cpp
    src/ui/styles/StyleSheet.cpp
    src/ui/widgets/Card.cpp
    src/ui/widgets/StatusBadge.cpp
    src/ui/widgets/TopBar.cpp
    src/ui/widgets/NavBar.cpp
    src/ui/widgets/MotorStatusCard.cpp
    src/ui/widgets/ElectricalStatusCard.cpp
    src/ui/widgets/ControlOutputCard.cpp
    src/ui/widgets/FOCDebugCard.cpp
    src/ui/pages/DashboardPage.cpp
    src/ui/pages/MotorAPage.cpp
    src/ui/pages/MotorBPage.cpp
    src/ui/pages/WaveformPage.cpp
    src/ui/pages/TuningPage.cpp
    src/ui/pages/SystemPage.cpp
    # 核心功能层
    src/core/SerialPortManager.cpp
    src/core/DataParser.cpp
)

# 头文件列表（用于MOC）
set(PROJECT_HEADERS
    src/ui/MainWindow.h
    src/ui/styles/StyleSheet.h
    src/ui/widgets/Card.h
    src/ui/widgets/StatusBadge.h
    src/ui/widgets/TopBar.h
    src/ui/widgets/NavBar.h
    src/ui/widgets/MotorStatusCard.h
    src/ui/widgets/ElectricalStatusCard.h
    src/ui/widgets/ControlOutputCard.h
    src/ui/widgets/FOCDebugCard.h
    src/ui/pages/DashboardPage.h
    src/ui/pages/MotorAPage.h
    src/ui/pages/MotorBPage.h
    src/ui/pages/WaveformPage.h
    src/ui/pages/TuningPage.h
    src/ui/pages/SystemPage.h
    src/core/SerialPortManager.h
    src/core/DataParser.h
    src/models/FOCParameters.h
    src/models/ControlMode.h
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(FOC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    if(ANDROID)
        add_library(FOC SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
        )
    else()
        add_executable(FOC
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
        )
    endif()
endif()

target_link_libraries(FOC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort)

target_include_directories(FOC PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(QT_VERSION_MAJOR EQUAL 6)
    if(Qt6_VERSION VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FOC)
    endif()
endif()
set_target_properties(FOC PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FOC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FOC)
endif()
