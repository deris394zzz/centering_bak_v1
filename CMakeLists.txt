cmake_minimum_required(VERSION 3.8)
project(centering_bak)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(msg_bank REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)

find_package(yaml REQUIRED)
find_package(yaml-cpp REQUIRED)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) 
include_directories(${OpenCV_INCLUDE_DIRS})



# find_package(<dependency> REQUIRED)

add_executable(centering_bak src/centering_bak.cpp)
ament_target_dependencies(centering_bak rclcpp msg_bank OpenCV cv_bridge yaml yaml-cpp)  # CHANGE
target_link_libraries(centering_bak ${OpenCV_LIBS})
target_link_libraries(centering_bak centering)

install(TARGETS
  centering_bak
  DESTINATION lib/${PROJECT_NAME})

target_link_libraries(centering_bak yaml-cpp)

add_library(centering STATIC
    lib/centering.cpp
)

target_include_directories(centering
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS
    centering
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/lib
)

install(DIRECTORY lib/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
