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
    abstractrepository.h abstractrepository.cpp
    songrepository.h songrepository.cpp
    playlistrepository.h playlistrepository.cpp
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
