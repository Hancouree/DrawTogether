cmake_minimum_required(VERSION 3.16)

project(DrawTogether VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick WebSockets)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDrawTogether
    main.cpp
)

qt_add_qml_module(appDrawTogether
    URI DrawTogether
    QML_FILES
        Main.qml
    SOURCES
        logic/logic.h
        logic/logic.cpp
        QML_FILES components/LoginPage.qml
        SOURCES
        QML_FILES components/ConnectionPage.qml
        QML_FILES
        QML_FILES reusable/RectButton.qml
        QML_FILES reusable/Spinner.qml
)

set_target_properties(appDrawTogether PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDrawTogether
    PRIVATE
        Qt6::Quick
        Qt6::WebSockets
)

include(GNUInstallDirs)
install(TARGETS appDrawTogether
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
