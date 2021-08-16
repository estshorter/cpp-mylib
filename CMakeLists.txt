cmake_minimum_required(VERSION 3.14)

project(test)
add_executable(test main.cpp misc.h)

find_package(ICU COMPONENTS uc i18n REQUIRED)

target_link_libraries(test ICU::uc ICU::i18n)

target_compile_options(test PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /source-charset:utf-8 /Zc:__cplusplus /Zc:preprocessor>
)
target_compile_features(test PRIVATE cxx_std_17)

set(resources ${CMAKE_CURRENT_LIST_DIR}/file_sjis.txt ${CMAKE_CURRENT_LIST_DIR}/file_utf8.txt)
add_custom_command(TARGET test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${resources} $<TARGET_FILE_DIR:test>)
