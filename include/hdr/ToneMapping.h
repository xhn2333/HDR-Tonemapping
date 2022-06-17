//
// Created by honnen on 22-5-21.
//

#ifndef HDRPLUS_TONEMAPPING_H
#define HDRPLUS_TONEMAPPING_H


#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/photo.hpp>
#include "hdr.hpp"

class ToneMapping {
public:
    static std::vector<cv::Mat> toneMappingPipeline(std::vector<cv::Mat>images, float Lmin, float Lmax);
    static cv::Mat toneMapping(cv::Mat tmp_image, float Lmin, float Lmax, float Ev);
};


#endif //HDRPLUS_TONEMAPPING_H
