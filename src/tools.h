#pragma once

#include <boost/lexical_cast.hpp>

uint32_t str2ip(const std::string& str) {
    uint32_t ip = 0;
    uint32_t part = 0;
    uint32_t part_count = 0;

    for(const auto& c : str) {
        if(std::isdigit(c))
            part = (part * 10) + (c - '0');
        else if(c == '.') {
            if(++part_count == 4)
                break;
            ip = (ip << 8) + part;
            part = 0;
        } else
            break;
    }
    ip = (ip << 8) + part;
    if(++part_count < 4)
        ip = ip << ((4 - part_count) << 3);

    return ip;
}

std::string ip2str(const uint32_t &ip) {
    return boost::lexical_cast<std::string>(ip>>24) + "." +
        boost::lexical_cast<std::string>((ip>>16)&0xff) + "." +
        boost::lexical_cast<std::string>((ip>>8)&0xff) + "." +
        boost::lexical_cast<std::string>(ip&0xff);
}

auto ips_read(std::istream& in)
{
    std::vector< uint32_t > ips;
    for(std::string line; std::getline(in, line);) {
        ips.push_back(str2ip(line));
    }

    return ips;
}

void ips_dump(std::ostream& out, const std::vector<uint32_t> &ips)
{
    for(const auto &ip : ips)
        out << ip2str(ip) << std::endl;
}

template<typename Predicat>
auto ips_filter(const std::vector<uint32_t> &ips, Predicat predicat)
{
    std::vector<uint32_t> filtered;
    std::copy_if(ips.begin(), ips.end(), filtered.begin(), predicat);
    return filtered;
}