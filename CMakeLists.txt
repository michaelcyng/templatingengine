cmake_minimum_required (VERSION 2.8.2)

project (TemplatingEngine)

set(CMAKE_CXX_STANDARD 14)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

include_directories(./include)
file(GLOB templatingenginesrc "./src/*")
add_library(templatingengine STATIC ${templatingenginesrc})

add_executable(EscapeStreamTest tests/EscapeStreamTest.cpp)
target_link_libraries(EscapeStreamTest gtest gtest_main templatingengine)

add_executable(LoopRendererTest tests/LoopRendererTest.cpp)
target_link_libraries(LoopRendererTest gtest gtest_main templatingengine)

add_executable(PlainTextRendererTest tests/PlainTextRendererTest.cpp)
target_link_libraries(PlainTextRendererTest gtest gtest_main templatingengine)

add_executable(RendererChainTest tests/RendererChainTest.cpp)
target_link_libraries(RendererChainTest gtest gtest_main templatingengine)

add_executable(RendererChainBuilderTest tests/RendererChainBuilderTest.cpp)
target_link_libraries(RendererChainBuilderTest gtest gtest_main templatingengine)

add_executable(TemplateTest tests/TemplateTest.cpp)
target_link_libraries(TemplateTest gtest gtest_main templatingengine)

add_executable(TemplateTokenizerTest tests/TemplateTokenizerTest.cpp)
target_link_libraries(TemplateTokenizerTest gtest gtest_main templatingengine)

add_executable(TokenListBuilderTest tests/TokenListBuilderTest.cpp)
target_link_libraries(TokenListBuilderTest gtest gtest_main templatingengine)

add_executable(TokenUtilsTest tests/TokenUtilsTest.cpp)
target_link_libraries(TokenUtilsTest gtest gtest_main templatingengine)

add_executable(ValueListTest tests/ValueListTest.cpp)
target_link_libraries(ValueListTest gtest gtest_main templatingengine)

add_executable(VariableRendererTest tests/VariableRendererTest.cpp)
target_link_libraries(VariableRendererTest gtest gtest_main templatingengine)
