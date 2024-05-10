cmake_minimum_required(VERSION 3.5)

project(luaclip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(Lua 5.1 EXACT REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(${LUA_INCLUDE_DIR})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(include)

add_library(luaclip SHARED src/buffer.cpp src/luaclip.cpp )

target_link_libraries(luaclip ${LUA_LIBRARIES} ${GLIB_LIBRARIES} ${X11_LIBRARIES} ${XFIXES_LIBRARIES} Xfixes Threads::Threads)
