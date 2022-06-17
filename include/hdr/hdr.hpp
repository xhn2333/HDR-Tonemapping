//
// Created by schrodinger on 5/15/22.
//

#ifndef HDRPLUS_HDR_HPP
#define HDRPLUS_HDR_HPP

#include <boost/system.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>

namespace hdr {
    using namespace cv;

    struct FramesWithMetadata {
        std::vector<Mat> frames;
        std::vector<float> exposure_time;
    };

    Mat hdr_pipeline(const FramesWithMetadata& data);
}
#endif //HDRPLUS_HDR_HPP
