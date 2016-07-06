cmake_minimum_required (VERSION 2.6)
project (sonarlog2video)

include ( "${WORKSPACE_DIR}/base/cmake/Dependencies.cmake"  )

add_project_dependency (base       ${WORKSPACE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_project_dependency (sonar_util ${WORKSPACE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_project_dependency (rock_util  ${WORKSPACE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

include ( FindPkgConfig  )

find_package ( Boost COMPONENTS system filesystem program_options REQUIRED )
find_package ( Eigen3 REQUIRED )
find_package ( OpenCV REQUIRED )

pkg_check_modules(pocolog_cpp pocolog_cpp)

file ( GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${base_INCLUDE_DIR}
    ${rock_util_INCLUDE_DIR}
    ${sonar_util_INCLUDE_DIR}
    ${ROCK_ROBOTICS_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
)

link_directories (
    ${ROCK_ROBOTICS_LIBRARY_DIR}
)

add_executable (
    sonarlog2video
    ${SOURCES}
)

target_link_libraries (
    sonarlog2video
    rock_util
    ${Boost_LIBRARIES}
    ${pocolog_cpp_LIBRARIES}
    ${OpenCV_LIBS}
)
