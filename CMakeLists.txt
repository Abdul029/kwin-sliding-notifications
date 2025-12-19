cmake_minimum_required(VERSION 3.16)
project(kwin_sliding_test)

# 1. Setup ECM and standard KDE install locations
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

# 2. Find Dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(KWin REQUIRED)
find_package(KF6 REQUIRED COMPONENTS Config)

# 3. Define the Plugin
add_library(kwin_final_sliding MODULE SlidingEffect.cpp)

# 4. Use the correct exported targets
# Note: KWinEffects is the standard target name for Plasma 6
target_link_libraries(kwin_final_sliding
    KWinEffects
    Qt6::Core
    Qt6::Gui
    KF6::ConfigCore
)

# 5. Use the standardized install variable from KDEInstallDirs
install(TARGETS kwin_final_sliding
        DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/kwin/effects/plugins)
