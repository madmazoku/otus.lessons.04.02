#include <iostream>
#include <spdlog/spdlog.h>

#include <boost/program_options.hpp>

#include "../bin/version.h"

#include "tools.h"

int main(int argc, char** argv) {
    auto console = spdlog::stdout_logger_st("console");
    console->info("Welcome to spdlog!");

    boost::program_options::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "print usage message")
        ("version,v", "print version number")
        ("filter,f", "set filter");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {  
        std::cout << "Usage " << *argv[0] << std::endl;
        std::cout << desc << std::endl;
    } else if (vm.count("version")) {  
        std::cout << "Version: " << version() << std::endl;
    } else {
        auto ips = ip_read(std::cin);

        std::cout << "Read ips: " << ips.size() << std::endl;

        // auto lambda = [](auto a, auto b)->bool{ a < b; };
        // std::sort(ips.begin(), ips.end(), [](auto a, auto b)->bool{ 
        //     auto ia = a.begin();
        //     auto ia = a.begin();
        // });

        ip_dump(std::cout, ips);
    }

    console->info("Goodbye!");

    return 0;
}