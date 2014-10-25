FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/people_msgs/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/people_msgs/msg/__init__.py"
  "../src/people_msgs/msg/_PositionMeasurement.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
