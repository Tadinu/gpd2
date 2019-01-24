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

/** HandGeometry class
 *
 * \brief Store robot hand geometry
 *
 * This class stores parameters which define the geometry of the robot hand.
 *
 */
class HandGeometry {
 public:
  HandGeometry();

  HandGeometry(double finger_width, double outer_diameter, double hand_depth,
               double hand_height, double init_bite);

  HandGeometry(const std::string& filepath);

  double finger_width_;    ///< the width of the robot hand fingers
  double outer_diameter_;  ///< the maximum robot hand aperture
  double depth_;           ///< the hand depth (length of fingers)
  double
      height_;  ///< the hand extends plus/minus this value along the hand axis
  double init_bite_;  ///< the minimum object height
};

std::ostream& operator<<(std::ostream& stream,
                         const HandGeometry& hand_geometry);

}  // namespace candidate
}  // namespace gpd

#endif /* HAND_GEOMETRY_H_ */
