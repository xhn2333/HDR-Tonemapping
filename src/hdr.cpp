#include <hdr/hdr.hpp>

namespace hdr {
    Mat hdr_pipeline(const FramesWithMetadata& data) {
        Mat response;
        auto calibrate = createCalibrateDebevec();
        calibrate->process(data.frames, response, data.exposure_time);
        Mat hdr;
        auto merger = createMergeDebevec();
        merger->process(data.frames, hdr, data.exposure_time, response);
        return hdr;
    }
}


