#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "ArgumentParser.hpp"

using namespace boost;

namespace sonarlog2video {

ArgumentParser::ArgumentParser()
    : input_file_("")
    , output_file_("")
    , stream_name_("")
{
}

ArgumentParser::~ArgumentParser() {
}

bool ArgumentParser::run(int argc, char **argv) {
    std::string app_name = boost::filesystem::basename(argv[0]);

    program_options::options_description desc("create video from sonar scan log");

    desc.add_options()
        ("input-file,i", program_options::value<std::string>()->required(), "the input file path")
        ("output-file,o", program_options::value<std::string>(), "the output file path")
        ("stream-name,s", program_options::value<std::string>()->default_value("sonar.sonar_scan_samples"), "the stream name")
        ("help,h", "show the command line description");

    program_options::positional_options_description pd;

    pd.add("input-file", 1);
    pd.add("output-file", 2);

    program_options::variables_map vm;

    try {
        program_options::store(program_options::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return false;
        }

        if (vm.count("input-file")) {
            input_file_ = vm["input-file"].as<std::string>();

            if (!file_exists(input_file_)){
                std::cerr << "ERROR: input-file not found" << std::endl;
                return false;
            }
        }

        if (vm.count("output-file")) {
            output_file_ = vm["output-file"].as<std::string>();
        }
        else {
            output_file_ = get_filename(input_file_) + ".mpg";
        }

        stream_name_ = vm["stream-name"].as<std::string>();

        program_options::notify(vm);
    } catch (boost::program_options::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return false;
    }

    return true;
}

bool ArgumentParser::file_exists(std::string filename) {

    if (!filesystem::exists(filename) &&
        !filesystem::exists(filesystem::path(filesystem::current_path()).string() + "/" + filename)){
        return false;
    }

    return true;

}

std::string ArgumentParser::get_filename(std::string file_path) {
    filesystem::path p(file_path);
    return p.stem().string();
}


} /* namespace sonarlog2video */
