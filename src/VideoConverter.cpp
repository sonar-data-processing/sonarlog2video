#include <iostream>
#include <vector>
#include <ctime>
#include <base/Time.hpp>
#include "sonarlog2video/VideoConverter.hpp"
#include "rock_util/SonarSampleConverter.hpp"
#include "rock_util/SonarInfo.hpp"
#include "base/MathUtil.hpp"

using namespace base;
using namespace sonar_util;
using namespace rock_util;

namespace sonarlog2video {

VideoConverter::VideoConverter(
    const std::string& input_file_path, 
    const std::string& output_file_path, 
    const std::string& stream_name,
    int frame_width,
    int frame_height)
    : output_file_path_(output_file_path)
    , frame_height_(frame_height)
    , frame_width_(frame_width)
    , reader_(input_file_path)
{
    stream_ = reader_.stream(stream_name);
}

void VideoConverter::convert() {
    try {
        
        stream_.reset();

        std::vector<int> beam_mapping;
        
        base::samples::Sonar last_sample;
        base::samples::Sonar sample;

        cv::Mat sonar_polar_image;
        cv::VideoWriter output_video;
        cv::Size frame_size;

        memset(&last_sample , 0, sizeof(last_sample));
        memset(&sample , 0, sizeof(sample));

        if (next_sonar_sample(sample)) {
            frame_size = get_frame_size(sample);
            output_video.open(output_file_path_, CV_FOURCC('P','I','M','1'), 25, frame_size, true);

            if (!output_video.isOpened()) {
                throw std::runtime_error("cannot open the output video.");
            }

            sonar_polar_image = SonarSampleConverter::convert2polar(sample, frame_size.width, frame_size.height, beam_mapping, color_palletes::HEATMAP_PALLETE);
            last_sample = sample;
            output_video << sonar_polar_image;
    
            while (stream_.current_sample_index() < stream_.total_samples()) {
                if (next_sonar_sample(sample)) {

                    if (!compare_sonar_data(sample, last_sample)) {
                        beam_mapping = SonarSampleConverter::generate_beam_mapping_from_cartesian(sample, frame_size.width, frame_size.height);
                    }

                    sonar_polar_image = SonarSampleConverter::convert2polar(sample, frame_size.width, frame_size.height, beam_mapping, color_palletes::HEATMAP_PALLETE);
                    output_video << sonar_polar_image;
                    last_sample = sample;
                }

                printf("[%3d%%] Processing frame %ld of %ld\n",
                    MathUtil::percentage(stream_.current_sample_index()+1, stream_.total_samples()), 
                    stream_.current_sample_index()+1, stream_.total_samples());
            }
        }
        
    } catch (...) {
        throw;
    }
}

bool VideoConverter::next_sonar_sample(base::samples::Sonar& sample) {
    return stream_.next<base::samples::Sonar>(sample);
}

bool VideoConverter::compare_sonar_data(const base::samples::Sonar& sonar0, const base::samples::Sonar& sonar1) {

    return (sonar0.beam_count == sonar1.beam_count &&
        sonar0.bin_count == sonar1.bin_count &&
        sonar0.beam_width == sonar1.beam_width &&
        sonar0.beam_height == sonar1.beam_height &&
        sonar0.bearings.size() == sonar1.bearings.size() &&
        std::equal(sonar0.bearings.begin(), sonar0.bearings.end(), sonar1.bearings.begin()));
}

cv::Size VideoConverter::get_frame_size(const base::samples::Sonar& sample) {
    cv::Size frame_size;

    frame_size.height = (frame_height_ > 0) 
                            ? frame_height_ 
                            : DEFAULT_IMAGE_HEIGHT;

    frame_size.width  = (frame_width_ > 0)
                            ? frame_width_ 
                            : MathUtil::aspect_ratio_width(sample.bearings[sample.bearings.size()-1].rad, frame_size.height);

    return frame_size;    
}


} /* namespace sonarlog2video */
