cmake_minimum_required(VERSION 3.16)

project(Chromasound-Updater VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LINUX_ARM OFF)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

if (NOT LINUX_ARM)
    include_directories("/usr/x86_64-pc-linux-gnu/usr/lib/gcc/x86_64-pc-linux-gnu/14/include/g++-v14/")
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        stk500v2.cpp
        stk500v2.h
        serial.h
        serial.cpp
        programmer.h
        programmer.cpp
        chromasound.h
        chromasound.cpp
        filedownloader.h
        filedownloader.cpp
)

if (WIN32)
    set (PROJECT_SOURCES
        ${PROJECT_SOURCES}
        windowsserial.h
        windowsserial.cpp)
else()
    set (PROJECT_SOURCES
        ${PROJECT_SOURCES}
        posixserial.h
        posixserial.cpp)

    if (LINUX)
        set (PROJECT_SOURCES
            ${PROJECT_SOURCES}
            gpio.cpp
            gpio.h)
    endif()
endif()


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if (WIN32)
        set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/chromasound-updater.rc")
        qt_add_executable(Chromasound-Updater
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources.qrc
            ${app_icon_resource_windows}
        )
    elseif (LINUX)
        qt_add_executable(Chromasound-Updater
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources.qrc
        )
    elseif (APPLE)
        set(MACOSX_BUNDLE_ICON_FILE chromasound-updater.icns)
        set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/chromasound-updater.icns")
        set_source_files_properties(${app_icon_macos} PROPERTIES
                   MACOSX_PACKAGE_LOCATION "Resources")
        qt_add_executable(Chromasound-Updater
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources.qrc
            ${app_icon_macos}
        )
    endif()
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Chromasound-Updater APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Chromasound-Updater SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Chromasound-Updater
            ${PROJECT_SOURCES}
            resources.qrc
        )
    endif()
endif()

target_link_libraries(Chromasound-Updater PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Chromasound-Updater)
endif()
set_target_properties(Chromasound-Updater PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Chromasound-Updater
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Chromasound-Updater)
endif()
