#include <iostream>
#include "sonarlog2video/ArgumentParser.hpp"
#include "sonarlog2video/VideoConverter.hpp"

using namespace sonarlog2video;

int main(int argc, char **argv) {

    ArgumentParser argument_parser;
    if (argument_parser.run(argc, argv)) {
        std::cout << "Convert Sonarlog 2 Video" << std::endl;
        std::cout << "intput-file: " << argument_parser.input_file()  << std::endl;
        std::cout << "output-file: " << argument_parser.output_file() << std::endl;
        std::cout << "stream-name: " << argument_parser.stream_name() << std::endl;
        VideoConverter video_converter(argument_parser.input_file(), argument_parser.output_file(), argument_parser.stream_name());
        video_converter.convert();
    }

    return 0;
}
