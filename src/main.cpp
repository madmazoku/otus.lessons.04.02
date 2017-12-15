#include <iostream>

#include <boost/program_options.hpp>

#include "../bin/version.h"

#include "tools.h"

int main(int argc, char** argv) {
    boost::program_options::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "print usage message")
        ("version,v", "print version number")
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

        std::sort(ips.begin(), ips.end(), [](auto a, auto b)->bool{ return a > b; });

        switch(vm["filter"].as<uint32_t>()) {
            case 1:
                ips_dump(std::cout, ips_filter(ips, [](auto a){ return (a>>24) == 1 ;}));
            break;
            case 2:
                ips_dump(std::cout, ips_filter(ips, [](auto a){ return (a>>16) == ((46<<8)|70) ;}));
            break;
            case 3:
                ips_dump(std::cout, ips_filter(ips, [](auto a){ return (a>>24) == 46 || ((a>>16)&0xff) == 46 || ((a>>8)&0xff) == 46 || (a&0xff) == 46 ;}));
            break;
            default:
                ips_dump(std::cout, ips);
            break;
        }

    }

    return 0;
}