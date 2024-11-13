cmake_minimum_required(VERSION 3.16)

project(chemical-searcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Gui)

file(GLOB WIDGET_SOURCES widgets/*.cpp)
file(GLOB WIDGET_HEADERS widgets/*.h)
file(GLOB WIDGET_UIS widgets/*.ui)

file(GLOB INCLUDE_SOURCES include/**/*.cpp)
file(GLOB INCLUDE_HEADERS include/**/*.h)

set(PROJECT_SOURCES
        main.cpp
        ${WIDGET_SOURCES}
		${WIDGET_HEADERS}
		${WIDGET_UIS}
		${INCLUDE_SOURCES}
		${INCLUDE_HEADERS}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chemical-searcher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(chemical-searcher SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(chemical-searcher
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(chemical-searcher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(chemical-searcher PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(chemical-searcher PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

target_link_libraries(chemical-searcher PRIVATE ${PROJECT_SOURCE_DIR}/lib/discord_game_sdk.dll.lib)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chemical-searcher)
endif()

set_target_properties(chemical-searcher PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS chemical-searcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chemical-searcher)
endif()
