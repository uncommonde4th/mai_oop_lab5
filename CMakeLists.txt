cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER /usr/bin/g++-14)

project(lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

add_executable(${CMAKE_PROJECT_NAME}_exe src/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME}_exe PRIVATE include)
# Добавление тестов
#enable_testing()

#add_executable(tests test/test1.cpp)
#target_link_libraries(tests PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest_main gtest pthread)

# Добавление тестов в тестовый набор
#add_test(NAME MyProjectTests COMMAND tests)
