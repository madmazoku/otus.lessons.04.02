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
        ("version,v", "print version number");
        ("filter,f", boost::program_options::value<uint32_t>()->default_value(0), "set filter type");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {  
        std::cout << "Usage " << *argv[0] << std::endl;
        std::cout << desc << std::endl;
    } else if (vm.count("version")) {  
        std::cout << "Version: " << version() << std::endl;
    } else {
        auto ips = ips_read(std::cin);

        std::cout << "Read ips: " << ips.size() << std::endl;

        std::sort(ips.begin(), ips.end(), [](auto a, auto b)->bool{ return a > b; });

        // auto ips_f1 = ips_filter(ips, [](auto a){ return false;});
        // decltype(ips) ips_f1;
        // std::copy_if(ips.begin(), ips.end(), ips_f1.begin(), [](auto a){ return false; });

        switch(vm["filter"].as<uint32_t>()) {
            case 1:
                std::cout << "Filtered ips list:" << std::endl;
                ips_dump(std::cout, ips_filter(ips, [](auto a){ return (a>>24) == 46 ;}));
            break;
            case 2:
            break;
            case 3:
            break;
            default:
                std::cout << "Full ips list:" << std::endl;
                ips_dump(std::cout, ips);
            break;
        }

    }

    console->info("Goodbye!");

    return 0;
}