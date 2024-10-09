cmake_minimum_required(VERSION 3.5)

project(head_unit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        spotify.h
        spotify.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(head_unit
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        spotify.h spotify.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET head_unit APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(head_unit SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(head_unit
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(head_unit PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# WebEngineWidgets
find_package(Qt6 REQUIRED COMPONENTS WebEngineWidgets)
target_link_libraries(head_unit PRIVATE Qt6::WebEngineWidgets)

# QNetworkAccessManager, QNetworkReply
find_package(Qt6 REQUIRED COMPONENTS Network)
target_link_libraries(head_unit PRIVATE Qt6::Network)

# QJsonDocument
find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(head_unit PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.head_unit)
endif()
set_target_properties(head_unit PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS head_unit
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(head_unit)
endif()
