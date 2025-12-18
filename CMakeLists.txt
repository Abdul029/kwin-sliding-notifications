cmake_minimum_required(VERSION 3.16)
project(kwin_sliding_test)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(KWin REQUIRED)
find_package(KF6 REQUIRED COMPONENTS Config)

add_library(kwin_final_sliding MODULE SlidingEffect.cpp)
target_link_libraries(kwin_final_sliding KWin::Effect Qt6::Core Qt6::Gui)

install(TARGETS kwin_final_sliding DESTINATION ${KWIN_PLUGIN_INSTALL_DIR}/kwin/effects/plugins)
