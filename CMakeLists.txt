cmake_minimum_required(VERSION 3.21)

project(Inventario VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# --- Archivos principales del proyecto ---
set(PROJECT_SOURCES
    main.cpp
    shelf.cpp
    panel.h
    main.ui
)

# --- Archivos del módulo de componentes ---
set(COMPONENT_SOURCES
    compItem/CompForm.h
    compItem/CompForm.cpp
    compItem/CompForm.ui
)

# --- Base de datos ---
set(DATABASE_SOURCES
    DataHub/DBControl.h
    DataHub/DBControl.cpp
)

# --- Modelos ---
set(MODEL_SOURCES
    model/CompList.h
    model/CompList.cpp
)

# --- Filtro ---
set(FILTER_SOURCES
    model/FiltProxy.h
    model/FiltProxy.cpp
)

# --- Unimos todos los archivos ---
set(ALL_SOURCES
    ${PROJECT_SOURCES}
    ${COMPONENT_SOURCES}
    ${DATABASE_SOURCES}
    ${MODEL_SOURCES}
    ${FILTER_SOURCES}
)

# --- Ejecutable ---
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Inventario
        MANUAL_FINALIZATION
        ${ALL_SOURCES}
    )
else()
    add_executable(Inventario ${ALL_SOURCES})
endif()

# --- Carpetas de includes necesarias ---
target_include_directories(Inventario PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}          # Raíz del proyecto
    ${CMAKE_CURRENT_SOURCE_DIR}/DataHub  # Para DBControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Database # Para DatabaseManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/model    # Para ComponentModel y FiltProxy
    ${CMAKE_CURRENT_SOURCE_DIR}/compItem # Para CompForm
)

target_link_libraries(Inventario PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# --- Propiedades para macOS/iOS ---
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Inventario)
endif()

set_target_properties(Inventario PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Inventario
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Inventario)
endif()