cmake_minimum_required (VERSION 2.6)
project (sonarlog2video)

list(APPEND CMAKE_MODULE_PATH $ENV{SONAR_TOOLKIT_PREFIX}/base/cmake)
list(APPEND CMAKE_MODULE_PATH $ENV{SONAR_TOOLKIT_PREFIX}/sonar_util/cmake)
list(APPEND CMAKE_MODULE_PATH $ENV{SONAR_TOOLKIT_PREFIX}/rock_util/cmake)

include ( FindPkgConfig )
include ( FindRock )

find_package ( Boost COMPONENTS system filesystem program_options REQUIRED )
find_package ( Eigen3 REQUIRED )
find_package ( OpenCV REQUIRED )

pkg_check_modules(pocolog_cpp pocolog_cpp)
 
file ( GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )
file ( GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp )

include_directories (
    ${ROCK_ROBOTICS_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_INSTALL_PREFIX}/include/sonar_toolkit
)

link_directories (
    ${ROCK_ROBOTICS_LIBRARY_DIR}
    ${CMAKE_INSTALL_PREFIX}/lib/sonar_toolkit
)

add_executable (
    sonarlog2video
    ${SOURCES}
)

target_link_libraries (
    sonarlog2video
    rock_util
    sonar_util
    ${Boost_LIBRARIES}
    ${pocolog_cpp_LIBRARIES}
    ${OpenCV_LIBS}
)

install(
    FILES ${HEADERS}
    DESTINATION include/sonar_toolkit/${PROJECT_NAME}
)

install(TARGETS
    sonarlog2video
    DESTINATION bin
)
