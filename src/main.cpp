#include <iostream>

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include "../bin/version.h"

#include "tools.h"

int main(int argc, char** argv)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "print usage message")
    ("version,v", "print version number");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
    } else if (vm.count("version")) {
        std::cout << "Build version: " << version() << std::endl;
        std::cout << "Boost version: " << (BOOST_VERSION / 100000) << '.' << (BOOST_VERSION / 100 % 1000) << '.' << (BOOST_VERSION % 100) << std::endl;
    } else {
        auto ips = ips_read(std::cin);

        std::sort(ips.begin(), ips.end(), [](const uint32_t &a, const uint32_t &b) {
            return a > b;
        });

        ips_dump(std::cout, ips); // full
        ips_dump(std::cout, ips_filter(ips, [](const uint32_t &a) {
            return (a>>24) == 1 ;
        })); // 1st byte == 1;
        ips_dump(std::cout, ips_filter(ips, [](const uint32_t &a) {
            return (a>>16) == ((46<<8)|70) ;
        })); // 1st byte = 36, 2nd byte = 70;
        ips_dump(std::cout, ips_filter(ips, [](const uint32_t &a) {
            return (a>>24) == 46 || ((a>>16)&0xff) == 46 || ((a>>8)&0xff) == 46 || (a&0xff) == 46 ;
        })); // any byte == 46
    }

    return 0;
}