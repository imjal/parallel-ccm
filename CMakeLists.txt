cmake_minimum_required(VERSION 3.16)
project(cpuSubdivide VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_add_executable(cpuSubdivide WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainview.cpp src/mainview.h
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    src/mesh.cpp src/mesh.h
    src/meshSubdivision.cpp
    src/meshinitializer.cpp src/meshinitializer.h
    src/meshrenderer.cpp src/meshrenderer.h
    src/objfile.cpp src/objfile.h
    src/quadmesh.cpp src/quadmesh.h
    src/renderer.h
    src/settings.cpp src/settings.h
    src/subdivider.cpp src/subdivider.h
    src/util.cpp src/util.h
)
target_include_directories(cpuSubdivide PRIVATE
    src
)

target_link_libraries(cpuSubdivide PRIVATE
    Qt::Core
    Qt::Gui
)

target_compile_options(cpuSubdivide
    -fopenmp
    -lpthread
)


# Resources:
set(resources_resource_files
    "src/shaders/default.frag"
    "src/shaders/default.vert"
    "src/shaders/edgeRefinement.glsl"
)

qt_add_resources(cpuSubdivide "resources"
    PREFIX
        "/"
    BASE
        "src"
    FILES
        ${resources_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(cpuSubdivide PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS cpuSubdivide
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
