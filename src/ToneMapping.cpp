//
// Created by honnen on 22-5-21.
//

#include "hdr/ToneMapping.h"
#include <iostream>
#include <filesystem>

std::vector<cv::Mat> ToneMapping::toneMappingPipeline(std::vector<cv::Mat>images, float Lmin, float Lmax){
    float Ev = 1.5;
    std::vector<cv::Mat>new_images;
    for(auto i:images){
        new_images.push_back(ToneMapping::toneMapping(i, Lmin, Lmax, Ev));
    }
    return new_images;
}

cv::Mat ToneMapping::toneMapping(cv::Mat tmp_image, float Lmin, float Lmax, float Ev){
    cv::Mat As, Ah, Am, B;
    cv::Mat image;
    tmp_image.convertTo(image, CV_32F);
    image /= 255;
    float maxVal = *(std::max_element(image.begin<float>(), image.end<float>()));
    float minVal = *(std::min_element(image.begin<float>(), image.end<float>()));
    As = cv::Mat::zeros(image.size(), image.type());
    Ah = cv::Mat::zeros(image.size(), image.type());
    Am = cv::Mat::zeros(image.size(), image.type());
    B = cv::Mat::zeros(image.size(), image.type());
    B.setTo(-minVal);
    float highlight_gain = std::pow(2.2, 1.0f - std::max(0.f, std::min(1.0f, (std::log(Ev) - Lmin)/(Lmax - Lmin) )));
//    std::cout << std::pow(2.2, 1.0f - std::max(0.f, std::min(1.0f, (std::log(Ev) - Lmin)/(Lmax - Lmin) ))) << std::endl;
    As.setTo(highlight_gain, image <= Lmin);
    cv::Mat temp = 1 + 0.2 * (As - 1) * (1 - (maxVal - minVal));
    Ah.setTo((1.2+highlight_gain)/2, image > Lmax);
    Ah = Ah.mul(temp);
    Am.setTo(1.2, (image <= Lmax) & (image > Lmin) );
    cv::Mat new_image = image.mul(As) + image.mul(Ah) + image.mul(Am) + B;
    new_image *= 255;
    new_image.convertTo(new_image, CV_8U);
    return new_image;
}
