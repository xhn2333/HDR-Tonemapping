//
// Created by schrodinger on 5/15/22.
//

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/photo.hpp>
#include <hdr/hdr.hpp>
int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);
    auto dirpath = argv[1];
    std::vector<float> time;
    std::vector<cv::Mat> raw_images;
    std::vector<cv::Mat> images;
    for (const auto & i : std::filesystem::directory_iterator { dirpath }) {
        if (!i.is_regular_file()) {
            continue;
        }
        auto path = i.path();
        auto filename = path.filename().string();
        auto separator = filename.find('-');
        auto a = std::stof(filename.substr(0, separator));
        auto b = std::stof(filename.substr(separator + 1));
        auto exposure_time = a / b;
        std::cout << path << ", time: " << exposure_time << std::endl;
        time.push_back(exposure_time);
        cv::Mat image = cv::imread(path.string(), -1);
        raw_images.push_back(image);
    }
    std::cout << raw_images.size() << std::endl;

    auto data = hdr::FramesWithMetadata { .frames = raw_images, .exposure_time = time };
    auto hdr = hdr::hdr_pipeline(data);
    cv::Mat output;
    cv::imwrite("test.hdr", hdr);
    {
        cv::Mat ldr;
        auto tonemap = cv::createTonemapDrago(1.4, 1.0, 0.7);
        tonemap->process(hdr, ldr);
        cv::imwrite("ldr.jpg", ldr * 3 * 255);

        cv::Mat fusion;
        auto merge_mertens = cv::createMergeMertens();
        merge_mertens->process(raw_images, fusion);
        imwrite("fusion.png", fusion * 255);
    }
}