#pragma once

#include <boost/lexical_cast.hpp>
#include <utility>
#include <iostream>

std::vector<std::string> split(const std::string &str, const char* d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

auto str2ip(const std::string& str = "")
{
    auto ip = split(str, ".\t");

    return std::make_tuple(
               ip.size() > 0 ? boost::lexical_cast<uint32_t>(ip[0]) : 0,
               ip.size() > 1 ? boost::lexical_cast<uint32_t>(ip[1]) : 0,
               ip.size() > 2 ? boost::lexical_cast<uint32_t>(ip[2]) : 0,
               ip.size() > 3 ? boost::lexical_cast<uint32_t>(ip[3]) : 0
           );
}

std::string ip2str(const decltype(str2ip()) ip)
{
    return boost::lexical_cast<std::string>(std::get<0>(ip)) + "." +
           boost::lexical_cast<std::string>(std::get<1>(ip)) + "." +
           boost::lexical_cast<std::string>(std::get<2>(ip)) + "." +
           boost::lexical_cast<std::string>(std::get<3>(ip));
}

auto ips_read(std::istream& in)
{
    std::vector<decltype(str2ip())> ips;
    for(std::string line; std::getline(in, line);)
        ips.push_back(str2ip(line));
    return ips;
}

void ips_dump(std::ostream& out, const decltype(ips_read(std::cin)) &ips)
{
    for(const auto &ip : ips)
        out << ip2str(ip) << std::endl;
}

// check sequental item equality
template<size_t N = 0> bool check_sequence(const decltype(str2ip("")) &ip)
{
    return true;
}
template<size_t N = 0, typename Octet, typename... Octets>
bool check_sequence(const decltype(str2ip("")) &ip, Octet octet, Octets... octets)
{
    return std::get<N>(ip) == octet && check_sequence<N+1>(ip, octets...);
}

// multi or operator
bool op_or()
{
    return false;
}
template<typename F, typename ...R>
bool op_or(F f, R ...r)
{
    return f || op_or(r...);
}

// check any item in sequency equality
template<size_t N, typename Octet >
struct check {
    static bool any(const decltype(str2ip("")) &ip, Octet octet)
    {
        return std::get<N>(ip) == octet || check<N+1, Octet>::any(ip, octet);
    }
};
template<typename Octet >
struct check<4, Octet> {
    static bool any(const decltype(str2ip("")) &ip, Octet octet)
    {
        return false;
    }
};
template<typename Octet>
bool check_any(const decltype(str2ip("")) &ip, Octet octet)
{
    return check<0,Octet>::any(ip, octet);
}