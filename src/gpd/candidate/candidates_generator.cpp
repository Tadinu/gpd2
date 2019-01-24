#include <gpd/candidate/candidates_generator.h>

namespace gpd {
namespace candidate {

CandidatesGenerator::CandidatesGenerator(
    const Parameters &params, const HandSearch::Parameters &hand_search_params)
    : params_(params) {
  Eigen::initParallel();

  hand_search_ = std::make_unique<candidate::HandSearch>(hand_search_params);
  plotter_ = std::make_unique<util::Plot>(hand_search_params.hand_axes_.size(),
                                          hand_search_params.num_orientations_);
}

void CandidatesGenerator::preprocessPointCloud(util::Cloud &cloud_cam) {
  const double VOXEL_SIZE = 0.003;

  std::cout << "Processing cloud with: " << cloud_cam.getCloudOriginal()->size()
            << " points.\n";

  cloud_cam.filterWorkspace(params_.workspace_);
  if (params_.voxelize_) {
    cloud_cam.voxelizeCloud(VOXEL_SIZE);
  }
  cloud_cam.calculateNormals(params_.num_threads_);
  cloud_cam.subsample(params_.num_samples_);

  // Calculate surface normals using integral images if possible.
  // if (cloud_cam.getCloudOriginal()->isOrganized() &&
  //     cloud_cam.getNormals().cols() == 0) {
  //   cloud_cam.calculateNormals(0);
  // }
  //
  // // perform statistical outlier removal
  // if (params_.remove_statistical_outliers_) {
  //   // Create the filtering object
  //   pcl::StatisticalOutlierRemoval<pcl::PointXYZRGBA> sor;
  //   sor.setInputCloud(cloud_cam.getCloudProcessed());
  //   sor.setMeanK(50);
  //   sor.setStddevMulThresh(1.0);
  //   sor.filter(*cloud_cam.getCloudProcessed());
  //   std::cout << "Cloud after removing statistical outliers: "
  //             << cloud_cam.getCloudProcessed()->size() << std::endl;
  // }
  //
  // // No indices into point cloud given
  // if (cloud_cam.getSampleIndices().size() == 0) {
  //   // 1. Workspace filtering
  //   cloud_cam.filterWorkspace(params_.workspace_);
  //   std::cout << "After workspace filtering: "
  //             << cloud_cam.getCloudProcessed()->size() << " points left.\n";
  //
  //   if (cloud_cam.getCloudProcessed()->size() == 0) {
  //     return;
  //   }
  //
  //   // 2. Voxelization
  //   if (params_.voxelize_) {
  //     printf("Voxelizing ...\n");
  //     cloud_cam.voxelizeCloud(VOXEL_SIZE);
  //     std::cout << "After voxelization: "
  //               << cloud_cam.getCloudProcessed()->size() << " points
  //               left.\n";
  //   }
  //
  //   // 3. Subsampling
  //   if (cloud_cam.getSamples().cols() > 0) {
  //     // 4. Calculate surface normals.
  //     if (cloud_cam.getNormals().cols() == 0) {
  //       cloud_cam.calculateNormals(params_.num_threads_);
  //     }
  //
  //     // 5. Subsample the remaining samples.
  //     cloud_cam.subsampleSamples(params_.num_samples_);
  //   } else {
  //     if (params_.num_samples_ > cloud_cam.getCloudProcessed()->size()) {
  //       std::vector<int> indices_all(cloud_cam.getCloudProcessed()->size());
  //       for (int i = 0; i < cloud_cam.getCloudProcessed()->size(); i++)
  //         indices_all[i] = i;
  //       cloud_cam.setSampleIndices(indices_all);
  //       std::cout << "Cloud is smaller than num_samples. Subsampled all "
  //                 << cloud_cam.getCloudProcessed()->size() << " points.\n";
  //     } else {
  //       cloud_cam.subsampleUniformly(params_.num_samples_);
  //       std::cout << "Subsampled " << params_.num_samples_
  //                 << " at random uniformly.\n";
  //     }
  //   }
  // }
  // // Indices into point cloud given
  // else {
  //   if (params_.num_samples_ > 0 &&
  //       params_.num_samples_ < cloud_cam.getSampleIndices().size()) {
  //     std::vector<int> indices_rand(params_.num_samples_);
  //     for (int i = 0; i < params_.num_samples_; i++)
  //       indices_rand[i] =
  //           cloud_cam.getSampleIndices()[rand() %
  //                                        cloud_cam.getSampleIndices().size()];
  //     cloud_cam.setSampleIndices(indices_rand);
  //     std::cout << "Subsampled " << indices_rand.size() << " indices.\n";
  //   } else {
  //     std::cout << "Using all " << cloud_cam.getSampleIndices().size()
  //               << " indices.\n";
  //   }
  // }
  //
  // // 4. Calculate surface normals.
  // if (cloud_cam.getNormals().cols() == 0) {
  //   cloud_cam.calculateNormals(params_.num_threads_);
  // }
  //
  // if (params_.plot_normals_) {
  //   plotter_->plotNormals(cloud_cam.getCloudProcessed(),
  //                         cloud_cam.getNormals());
  // }
}

std::vector<std::unique_ptr<Hand>> CandidatesGenerator::generateGraspCandidates(
    const util::Cloud &cloud_cam) {
  // Find sets of grasp candidates.
  std::vector<std::unique_ptr<HandSet>> hand_set_list =
      hand_search_->searchHands(cloud_cam);
  printf("Evaluated %d hand sets with %d potential hand poses.\n",
         (int)hand_set_list.size(),
         (int)(hand_set_list.size() * hand_set_list[0]->getHands().size()));

  if (params_.plot_all_grasps_) {
    const HandGeometry &hand_geom = hand_search_->getParams().hand_geometry_;
    plotter_->plotFingers3D(hand_set_list, cloud_cam.getCloudOriginal(),
                            "Grasp Poses", hand_geom);
  }

  // Extract the grasp candidates.
  std::vector<std::unique_ptr<Hand>> candidates;
  for (int i = 0; i < hand_set_list.size(); i++) {
    for (int j = 0; j < hand_set_list[i]->getHands().size(); j++) {
      if (hand_set_list[i]->getIsValid()(j)) {
        candidates.push_back(std::move(hand_set_list[i]->getHands()[j]));
      }
    }
  }
  std::cout << "Generated " << candidates.size() << " grasp candidates.\n";

  if (params_.plot_grasps_) {
    const HandGeometry &hand_geom = hand_search_->getParams().hand_geometry_;
    plotter_->plotFingers3D(candidates, cloud_cam.getCloudOriginal(),
                            "Grasp Candidates", hand_geom);
  }

  return candidates;
}

std::vector<std::unique_ptr<HandSet>>
CandidatesGenerator::generateGraspCandidateSets(const util::Cloud &cloud_cam) {
  // Find sets of grasp candidates.
  std::vector<std::unique_ptr<HandSet>> hand_set_list =
      hand_search_->searchHands(cloud_cam);

  if (params_.plot_grasps_) {
    const HandGeometry &hand_geom = hand_search_->getParams().hand_geometry_;
    plotter_->plotFingers3D(hand_set_list, cloud_cam.getCloudOriginal(),
                            "Grasp Candidates", hand_geom);
    plotter_->plotFingers3D(hand_set_list, cloud_cam.getCloudOriginal(),
                            "Grasp Candidates", hand_geom);
  }

  return hand_set_list;
}

std::vector<int> CandidatesGenerator::reevaluateHypotheses(
    const util::Cloud &cloud, std::vector<std::unique_ptr<Hand>> &grasps) {
  return hand_search_->reevaluateHypotheses(cloud, grasps);
}

}  // namespace candidate
}  // namespace gpd
