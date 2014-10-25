FILE(REMOVE_RECURSE
  "CMakeFiles/ROSBUILD_gencfg_cpp"
  "../cfg/cpp/leg_detector/LegDetectorConfig.h"
  "../docs/LegDetectorConfig.dox"
  "../docs/LegDetectorConfig-usage.dox"
  "../src/leg_detector/cfg/LegDetectorConfig.py"
  "../docs/LegDetectorConfig.wikidoc"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gencfg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
