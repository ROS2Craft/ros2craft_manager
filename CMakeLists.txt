cmake_minimum_required(VERSION 3.8)
project(ros2craft_manager)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(rclcpp_components REQUIRED)

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  std_msgs
  geometry_msgs
  nav_msgs
  ros2craft_interfaces
  Eigen3
  tf2
  tf2_ros
  rclcpp_components
)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Install launch files.
install(DIRECTORY
  launch
  scripts
  config
  DESTINATION share/${PROJECT_NAME}/
)

# Install Python modules
ament_python_install_package(${PROJECT_NAME})
# Install Python executables
# install(PROGRAMS
#   DESTINATION lib/${PROJECT_NAME}
# )

# Build the controller library
add_library(${PROJECT_NAME} SHARED
  src/Manager.cpp
  src/MCRobot.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

ament_target_dependencies(${PROJECT_NAME} ${THIS_PACKAGE_INCLUDE_DEPENDS})

# Build executables
add_executable(main_manager scripts/main_manager.cpp)
ament_target_dependencies(main_manager PUBLIC rclcpp std_msgs ros2craft_interfaces)
target_link_libraries(main_manager PUBLIC
    ${PROJECT_NAME})

add_executable(main scripts/main.cpp)
ament_target_dependencies(main PUBLIC rclcpp std_msgs ros2craft_interfaces)
target_link_libraries(main PUBLIC
    ${PROJECT_NAME})
    
# Install include directories
install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

# executables
install(TARGETS 
  main
  main_manager
  DESTINATION lib/${PROJECT_NAME}
)

rclcpp_components_register_node(
    ${PROJECT_NAME}
    PLUGIN "ros2craft::MCRobot"
    EXECUTABLE mcrobot
)

ament_export_targets(export_${PROJECT_NAME})
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
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
