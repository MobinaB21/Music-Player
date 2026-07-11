cmake_minimum_required(VERSION 3.19)
project(spotify LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(spotify
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    account.h account.cpp
    playlist.h playlist.cpp
    album.h album.cpp
    song.h song.cpp

    songrepository.h songrepository.cpp
    playlistrepository.h playlistrepository.cpp
    albumrepository.h albumrepository.cpp
    accountrepository.h accountrepository.cpp
    artistrepository.h artistrepository.cpp
    abstractrepository.h
    listenerrepository.h listenerrepository.cpp
    loginwindow.h loginwindow.cpp loginwindow.ui
    artistwindow.h artistwindow.cpp artistwindow.ui
    listenerwindow.h listenerwindow.cpp listenerwindow.ui
    registerwindow.h registerwindow.cpp registerwindow.ui
)

target_link_libraries(spotify
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS spotify
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET spotify
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
