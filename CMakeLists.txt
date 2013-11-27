cmake_minimum_required(VERSION 2.8)

project(LineSegmentation)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
include_directories(include)
set(HEADER_FILES  include/FilterSkull.h include/LineSegment.h include/ArcGroup.h)
set (SOURCES main.cpp FilterSkull.cpp LineSegment.cpp FindSegments.cpp LineExtraction.cpp ArcExtraction.cpp ThetaEstimation.cpp ArcGroup.cpp)
add_executable(LineSegmentation ${SOURCES} ${HEADER_FILES})

target_link_libraries(LineSegmentation ${ITK_LIBRARIES})