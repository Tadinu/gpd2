#include <string>
#include <fstream>
#include <gpd/grasp_detector.h>

namespace gpd {
namespace apps {
namespace detect_grasps {

void logGraspPoses(const std::vector<std::unique_ptr<candidate::Hand>>& grasp_list,
                   const std::string& pcd_filename)
{
    const std::string grasp_pose_file_path(pcd_filename + "_csv.txt");
    std::ofstream csvFile(grasp_pose_file_path);
    if(!csvFile.is_open()) {
        printf("Unable to open file [%s]", grasp_pose_file_path.c_str());
        return;
    }
    //
    csvFile << "trans_x,trans_y,trans_z,rot_w,rot_x,rot_y,rot_z,width," << "\n";
    //
    for(auto& grasp : grasp_list) {
        Eigen::VectorXd rotation(4);
        Eigen::Quaterniond quat(grasp->getOrientation());
        rotation << quat.w(), quat.x(), quat.y(), quat.z();

        csvFile << grasp->vectorToString(grasp->getPosition())
                << grasp->vectorToString(rotation)
#if 1
                << "0.06" << ',' << "\n";
#else
                << std::to_string(grasp->getGraspWidth()) << ',' << "\n";
#endif
    }

    csvFile.close();

    printf("WRITTEN TO FILE GRASP POSE LIST: %ld\n", grasp_list.size());
}

bool checkFileExists(const std::string &file_name) {
  std::ifstream file;
  file.open(file_name.c_str());
  if (!file) {
    std::cout << "File " + file_name + " could not be found!\n";
    return false;
  }
  file.close();
  return true;
}

int DoMain(int argc, char *argv[]) {
  // Read arguments from command line.
  if (argc < 3) {
    std::cout << "Error: Not enough input arguments!\n\n";
    std::cout << "Usage: detect_grasps CONFIG_FILE PCD_FILE [NORMALS_FILE]\n\n";
    std::cout << "Detect grasp poses for a point cloud, PCD_FILE (*.pcd), "
                 "using parameters from CONFIG_FILE (*.cfg).\n\n";
    std::cout << "[NORMALS_FILE] (optional) contains a surface normal for each "
                 "point in the cloud (*.csv).\n";
    return (-1);
  }

  std::string config_filename = argv[1];
  std::string pcd_filename = argv[2];
  if (!checkFileExists(config_filename)) {
    printf("Error: config file not found!\n");
    return (-1);
  }
  if (!checkFileExists(pcd_filename)) {
    printf("Error: PCD file not found!\n");
    return (-1);
  }

  // Read parameters from configuration file.
  util::ConfigFile config_file(config_filename);
  config_file.ExtractKeys();

  // Set the camera position. Assumes a single camera view.
  std::vector<double> camera_position =
      config_file.getValueOfKeyAsStdVectorDouble("camera_position",
                                                 "0.0 0.0 0.0");
  Eigen::Matrix3Xd view_points(3, 1);
  view_points << camera_position[0], camera_position[1], camera_position[2];

  // Load point cloud from file.
  util::Cloud cloud(pcd_filename, view_points);
  if (cloud.getCloudOriginal()->size() == 0) {
    std::cout << "Error: Input point cloud is empty or does not exist!\n";
    return (-1);
  }

  // Load surface normals from file.
  if (argc > 3) {
    std::string normals_filename = argv[3];
    cloud.setNormalsFromFile(normals_filename);
    std::cout << "Loaded surface normals from file: " << normals_filename
              << "\n";
  }

  GraspDetector detector(config_filename);

  // Preprocess the point cloud.
  detector.preprocessPointCloud(cloud);

  // If the object is centered at the origin, reverse all surface normals.
  bool centered_at_origin =
      config_file.getValueOfKey<bool>("centered_at_origin", false);
  if (centered_at_origin) {
    printf("Reversing normal directions ...\n");
    cloud.setNormals(cloud.getNormals() * (-1.0));
  }

  // Detect grasp poses.
  std::vector<std::unique_ptr<candidate::Hand>> graspCandidates =
    detector.detectGrasps(cloud);

  logGraspPoses(graspCandidates, pcd_filename);
  return 0;
}

}  // namespace detect_grasps
}  // namespace apps
}  // namespace gpd

int main(int argc, char *argv[]) {
  return gpd::apps::detect_grasps::DoMain(argc, argv);
}
