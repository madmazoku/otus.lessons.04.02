#pragma once

#include <boost/lexical_cast.hpp>

template <typename Td>
auto split(const std::string &str, Td d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

template<typename T>
auto splice(const T& col, const unsigned int& start, const unsigned int& end) 
{
    T before;
    T in;
    T after;
    unsigned int count = 0;
    for(const auto& e : col) {
        if(count < start) {
            before.push_back(e);
        } else if(count < end) {
            in.push_back(e);
        } else {
            after.push_back(e);
        }
    }
    return std::make_tuple(before, in, after);
}

template<typename Tc = std::initializer_list<std::string>, typename Td >
auto join(const Tc& col, const Td& d, const unsigned int& n = 0)
{
    unsigned int c = 0;
    std::string str;
    for(const auto& e : col) {
        if(!str.empty())
            str = str + boost::lexical_cast<std::string>(d);
        str = str + boost::lexical_cast<std::string>(e);
        if(n != 0 && ++c == n)
            break;
    }
    return str;
}

auto ip_read(std::istream& in)
{
    std::vector< std::vector<std::string> > ips;
    for(std::string line; std::getline(in, line);) {
        ips.push_back(split(line, ".\t"));
    }

    return ips;
}

void ip_dump(std::ostream& out, const std::vector< std::vector<std::string> > &ips)
{
    for(const auto &ip : ips)
        out << join(ip, '.', 4) << std::endl;
}