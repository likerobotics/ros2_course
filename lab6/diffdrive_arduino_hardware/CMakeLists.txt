cmake_minimum_required(VERSION 3.8)
project(diffdrive_arduino_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)

add_library(diffdrive_arduino_hardware SHARED
  src/diffdrive_arduino_hardware.cpp

  # --- vendored serial sources ---
  external/serial/src/serial.cc
  external/serial/src/impl/unix.cc
  external/serial/src/impl/list_ports/list_ports_linux.cc
)

# Public includes for your package
target_include_directories(diffdrive_arduino_hardware PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Private include path for vendored serial
target_include_directories(diffdrive_arduino_hardware PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/external/serial/include
)

ament_target_dependencies(diffdrive_arduino_hardware
  rclcpp
  hardware_interface
  pluginlib
)

# serial on Linux uses pthread and sometimes rt
target_link_libraries(diffdrive_arduino_hardware
  pthread
  rt
)

# Install configs / launch / urdf
install(
  DIRECTORY
    config launch urdf
  DESTINATION share/${PROJECT_NAME}
)

pluginlib_export_plugin_description_file(
  hardware_interface plugin_description.xml
)

install(
  TARGETS diffdrive_arduino_hardware
  EXPORT export_diffdrive_arduino_hardware
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  FILES plugin_description.xml
  DESTINATION share/${PROJECT_NAME}
)

ament_export_targets(export_diffdrive_arduino_hardware HAS_LIBRARY_TARGET)
ament_export_dependencies(
  rclcpp
  hardware_interface
  pluginlib
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
