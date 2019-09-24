#include <gpd/candidate/hand_geometry.h>

namespace gpd {
namespace candidate {

HandGeometry::HandGeometry() {}

HandGeometry::HandGeometry(double finger_width, double outer_diameter,
                           double hand_depth, double hand_height,
                           double init_bite)
    : params_(finger_width, outer_diameter, hand_depth, hand_height, init_bite) {}

HandGeometry::HandGeometry(const std::string &filepath) {
  util::ConfigFile config_file(filepath);
  config_file.ExtractKeys();
  params_.finger_width_ = config_file.getValueOfKey<double>("finger_width", 0.01);
  params_.outer_diameter_ =
      config_file.getValueOfKey<double>("hand_outer_diameter", 0.12);
  params_.depth_ = config_file.getValueOfKey<double>("hand_depth", 0.06);
  params_.height_ = config_file.getValueOfKey<double>("hand_height", 0.02);
  params_.init_bite_ = config_file.getValueOfKey<double>("init_bite", 0.01);
}

std::ostream &operator<<(std::ostream &stream,
                         const HandGeometry &hand_geometry) {
  stream << "============ HAND GEOMETRY ======================\n";
  stream << "finger_width: " << hand_geometry.params_.finger_width_ << "\n";
  stream << "hand_outer_diameter: " << hand_geometry.params_.outer_diameter_ << "\n";
  stream << "hand_depth: " << hand_geometry.params_.depth_ << "\n";
  stream << "hand_height: " << hand_geometry.params_.height_ << "\n";
  stream << "init_bite: " << hand_geometry.params_.init_bite_ << "\n";
  stream << "=================================================\n";
  return stream;
}

}  // namespace candidate
}  // namespace gpd
