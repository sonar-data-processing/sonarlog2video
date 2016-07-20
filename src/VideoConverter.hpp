#ifndef VideoConverter_hpp
#define VideoConverter_hpp

#include <string>
#include <base/samples/SonarScan.hpp>
#include <base/samples/Sonar.hpp>
#include <opencv2/opencv.hpp>

#include "rock_util/LogReader.hpp"

namespace sonarlog2video {

class VideoConverter {
public:
    VideoConverter(const std::string& input_file_path,
                   const std::string& output_file_path,
                   const std::string& stream_name,
                   int frame_width = -1,
                   int frame_height = -1);

    virtual ~VideoConverter() {}

    void convert();

private:

    bool compare_sonar_data(const base::samples::Sonar& sonar0,
                                   const base::samples::Sonar& sonar1);
    
    cv::Size get_frame_size(const base::samples::Sonar& sample);
    
    bool next_sonar_sample(base::samples::Sonar& sample);

    const std::string& output_file_path_;
    int frame_width_;
    int frame_height_;
    
    static std::string STREAM_TYPE_SONAR_SCAN;
    static std::string STREAM_TYPE_SONAR;
    
    rock_util::LogReader reader_;
    rock_util::LogStream stream_;

    static const int DEFAULT_IMAGE_HEIGHT = 600;

};

} /* namespace sonarlog2video */

#endif /* VideoConverter_hpp */
