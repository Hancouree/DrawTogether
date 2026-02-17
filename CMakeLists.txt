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
    VERSION 1.0
    QML_FILES
        Main.qml
        components/LoginPage.qml
        components/ConnectionPage.qml
        components/MenuPage.qml
        components/SearchRoomsPage.qml
        components/RoomSkeleton.qml
        components/Room.qml
        components/RoomCreationPage.qml
        components/Canvas.qml
        components/Notification.qml
        components/NotificationsContainer.qml
        components/RoomPage.qml
        reusable/RectButton.qml
        reusable/Spinner.qml
        reusable/CardPage.qml
        reusable/CustomCombobox.qml
        reusable/UserIcon.qml
        reusable/ConfirmationPopup.qml
        reusable/Toolbar.qml
        reusable/Toolbutton.qml
    SOURCES
        logic/logic.h logic/logic.cpp
        logic/roomsmodel.h logic/roomsmodel.cpp
        logic/fsm.h logic/fsm.cpp
        logic/requestmanager.h logic/requestmanager.cpp
        logic/participantsmodel.h logic/participantsmodel.cpp
        logic/roominfo.h logic/roominfo.cpp
        logic/notificationmanager.h logic/notificationmanager.cpp
        logic/notification.h logic/notification.cpp
        logic/drawingcanvas.h logic/drawingcanvas.cpp # Проверь, что файлы тут!
    RESOURCES
        icons/arrow.png
        icons/reload.png
        icons/kick.png
        icons/finish.png
        icons/diagonal-line.png icons/eraser.png
        icons/paint-brush.png
        RESOURCES
)

target_include_directories(appDrawTogether PRIVATE logic)

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
