set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)

project(detect)

find_package(OpenCV 4 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
