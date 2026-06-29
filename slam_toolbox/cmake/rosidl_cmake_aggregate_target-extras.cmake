# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target slam_toolbox::slam_toolbox
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${slam_toolbox_TARGETS}.
if(slam_toolbox_TARGETS AND NOT TARGET slam_toolbox::slam_toolbox)
  add_library(slam_toolbox::slam_toolbox INTERFACE IMPORTED)
  set_target_properties(slam_toolbox::slam_toolbox PROPERTIES
    INTERFACE_LINK_LIBRARIES "${slam_toolbox_TARGETS}")
endif()
