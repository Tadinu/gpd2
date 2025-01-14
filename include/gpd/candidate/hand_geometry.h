/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2018, Andreas ten Pas
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HAND_GEOMETRY_H_
#define HAND_GEOMETRY_H_

#include <gpd/util/config_file.h>

namespace gpd {
namespace candidate {

/**
 *
 * \brief Store robot hand geometry
 *
 * This class stores parameters which define the geometry of the robot hand.
 * This geometry is used to calculate the grasp candidates.
 *
 */
class HandGeometry {
 public:
  struct Parameters {
    double finger_width_;    ///< the width of the robot fingers
    double outer_diameter_;  ///< the width of the robot hand including fingers
    double depth_;           ///< the hand depth (length of fingers)
    double height_;     ///< the hand extends plus/minus this along the hand axis
    double init_bite_;  ///< the minimum object depth to be covered by the fingers
    double max_depth_;  ///< the maximum depth the hand could move on the object
    double deepen_step_; ///< the step size of deepening hand from init_bite_ to max_depth_

    //
    Parameters()
    : finger_width_(0.0),
      outer_diameter_(0.0),
      depth_(0.0),
      height_(0.0),
      init_bite_(0.0),
      max_depth_(0.0),
      deepen_step_(0.0) {}

    Parameters(double finger_width, double outer_diameter,
               double hand_depth, double hand_height,
               double init_bite, double max_depth, double deepen_step)
    : finger_width_(finger_width),
      outer_diameter_(outer_diameter),
      depth_(hand_depth),
      height_(hand_height),
      init_bite_(init_bite),
      max_depth_(max_depth),
      deepen_step_(deepen_step){}
  };

  /**
   * \brief Constructor.
   */
  HandGeometry();

  /**
   * \brief Constructor.
   * \param finger_width the width of a robot finger
   * \param outer_diameter the width of the robot hand (including fingers)
   * \param hand_depth the hand depth (length of finger)
   * \param hand_height the hand height: the hand extends plus/minus this
   * along the hand axis
   * \param init_bite the minimum object depth to be covered by the fingers
   * \param max_depth the maximum object depth to be covered by the fingers
   * \param deepen_step the deepening step as fingers moves from init_bite -> max_depth onto object
   */
  HandGeometry(double finger_width, double outer_diameter, double hand_depth,
               double hand_height, double init_bite, double max_depth, double deepen_step);

  /**
   * \brief Constructor that uses a given configuration file to read in the
   * parameters of the robot hand.
   * \param filepath the filepath to the configuration file
   */
  HandGeometry(const std::string &filepath);

  Parameters params_;
};

std::ostream &operator<<(std::ostream &stream,
                         const HandGeometry &hand_geometry);

}  // namespace candidate
}  // namespace gpd

#endif /* HAND_GEOMETRY_H_ */
