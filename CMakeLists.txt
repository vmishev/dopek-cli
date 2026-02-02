cmake_minimum_required(VERSION 3.16)
project(dopek LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core target: no UI, no Q_OBJECT, no resources
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


# ---------- executable ----------
add_executable(dopek
    main.cpp

)

# ---------- sources ----------
file(GLOB_RECURSE DOPEK_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/**/*.cpp
)
# ---------- headers ----------
file(GLOB_RECURSE DOPEK_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/**/*.h
)

target_sources(dopek PRIVATE
    ${DOPEK_SOURCES}
    ${DOPEK_HEADERS}
)
# ---------- include paths ----------
target_include_directories(dopek PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

# ---------- warnings (optional, but recommended) ----------
target_compile_options(dopek PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
target_link_libraries(dopek PRIVATE Qt${QT_VERSION_MAJOR}::Core)
