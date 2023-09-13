cmake_minimum_required(VERSION 3.26)

project(multiclickfix CXX)

add_library(mcfdll SHARED src/dll.cpp)
set_target_properties(mcfdll PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_features(mcfdll PUBLIC cxx_std_17)
target_include_directories(mcfdll PUBLIC inc)

add_executable(mcfexe WIN32 src/exe.cpp src/wingui.cpp)
target_link_libraries(mcfexe PRIVATE mcfdll)
target_compile_features(mcfexe PRIVATE cxx_std_17)

