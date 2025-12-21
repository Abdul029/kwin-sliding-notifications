cmake_minimum_required(VERSION 3.16)
project(kwin_final_sliding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

# 1. Find ECM first
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

# 2. FORCE QT 6
set(QT_MAJOR_VERSION 6)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

# 3. Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui)

# 4. find KF6
find_package(KF6 REQUIRED COMPONENTS CoreAddons WindowSystem Config WidgetsAddons)

# --- PORTABILITY LOGIC ---
find_path(KWIN_HEADERS_DIR
    NAMES effect/effect.h
    PATH_SUFFIXES kwin KWin
)

find_library(KWIN_LIBRARY
    NAMES kwin kwin6
    PATHS /usr/lib/x86_64-linux-gnu /usr/lib
)

add_library(kwin_final_sliding MODULE SlidingEffect.cpp)

target_include_directories(kwin_final_sliding SYSTEM PRIVATE ${KWIN_HEADERS_DIR})

target_link_libraries(kwin_final_sliding
    PRIVATE
    ${KWIN_LIBRARY}
    Qt6::Core
    KF6::CoreAddons
    KF6::WindowSystem
    KF6::ConfigCore
    KF6::WidgetsAddons
)

install(TARGETS kwin_final_sliding DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/kwin/effects/plugins)
