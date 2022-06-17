//
// Created by honnen on 22-5-21.
//
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/photo.hpp>
#include <hdr/hdr.hpp>
#include <hdr/ToneMapping.h>

#define TEMPORALWINDOWSIZE 7

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);
    auto dirpath = argv[1];
    std::vector<float> time;
    std::vector<cv::Mat> raw_images;
    std::vector<cv::Mat> images;
    std::vector<std::string> files;
    int numFrames = 0;

    for (const auto & i : std::filesystem::directory_iterator { dirpath }) {
        if (!i.is_regular_file()) {
            continue;
        }
        auto path = i.path();
        auto filename = path.filename().string();
//        auto separator = filename.find('-');
//        auto a = std::stof(filename.substr(0, separator));
//        auto b = std::stof(filename.substr(separator + 1));
//        auto exposure_time = a / b;
//        std::cout << path << ", time: " << exposure_time << std::endl;
//        time.push_back(exposure_time);
        files.push_back(filename);
        numFrames ++;
    }
    std::sort(files.begin(), files.end(), [](std::string &a, std::string & b) {
        if (a.length() != b.length())
            return a.length() < b.length();
        else{
            return a < b;
        }
    });

    for(auto i:files){
        auto path = dirpath + i;
        cv::Mat image = cv::imread(path, -1);
        image.convertTo(image, CV_32F);
        float maxVal = *(std::max_element(image.begin<float>(), image.end<float>()));
        image /= maxVal;
        image *= 255;
        image.convertTo(image, CV_8U);
        images.push_back(image);
        if (i == *(files.begin()) || i == *(files.end()-1)){
            for(int j = 0; j < TEMPORALWINDOWSIZE/2; j++)
                images.push_back(image);
        }
    }

    bool flag = true;
    std::vector<cv::Mat> hdr_images;
    cv::imwrite("hdr-before.png", images[15 + TEMPORALWINDOWSIZE/2]);
    for(int i = 0; i < numFrames; i++){
        cv::Mat denoised_img = images[i + TEMPORALWINDOWSIZE/2];
        cv::fastNlMeansDenoisingMulti(images, denoised_img, i + TEMPORALWINDOWSIZE/2, TEMPORALWINDOWSIZE, 3, 5, 7);
        cv::Mat tonemapped_img = ToneMapping::toneMapping(denoised_img, 0.3, 0.7, 1.5);
        hdr_images.push_back(tonemapped_img);
        std::cout << hdr_images.size() << std::endl;
//        cv::imshow("origin", images[i + TEMPORALWINDOWSIZE/2]);
        cv::imshow("test", denoised_img);
//        cv::waitKey(100);
    }
    cv::imwrite("hdr-after.png", hdr_images[15]);
    auto idx = hdr_images.begin();
    while(flag){
        if(idx == hdr_images.end()){
            idx = hdr_images.begin();
        }
        // std::cout << (*idx) << std::endl;
        cv::imshow("test", *idx);
        cv::waitKey(100);
        if(idx != hdr_images.end()){
            idx += 1;
        }
    }
}