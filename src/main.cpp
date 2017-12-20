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

        std::sort(std::begin(ips), std::end(ips));
        std::reverse(std::begin(ips), std::end(ips));

        ips_dump(std::cout, ips); // full

        decltype(ips) ips_filter_1;
        std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_1), [](const decltype(str2ip("")) &a) {
            return check_sequence(a, 1);
        });
        ips_dump(std::cout, ips_filter_1); // 1st byte == 1;

        decltype(ips) ips_filter_2;
        std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_2), [](const decltype(str2ip("")) &a) {
            return check_sequence(a, 46, 70);
        });
        ips_dump(std::cout, ips_filter_2); // 1st byte = 46, 2nd byte = 70;

        decltype(ips) ips_filter_3;
        std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_3), [](const decltype(str2ip("")) &a) {
            return check_any(a, 46);
        });
        ips_dump(std::cout, ips_filter_3); // any byte == 46
    }

    return 0;
}