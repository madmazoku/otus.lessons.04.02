#define BOOST_TEST_MODULE TestMain
#include <boost/test/unit_test.hpp>

#include "../bin/version.h"
#include "tools.h"

#include <iostream>

template<typename T>
boost::test_tools::predicate_result compare_collections(const T &got, const T &expect)
{
    if( got.size() != expect.size() ) {
        boost::test_tools::predicate_result res( false );
        res.message() << "Size mismatch [" << got.size() << "!=" << expect.size() << "]";
        return res;
    }

    auto g = got.begin();
    auto e = expect.begin();
    for(unsigned int i = 0; g != got.end(); ++g, ++e, ++i) {
        if(*g != *e) {
            boost::test_tools::predicate_result res( false );
            res.message() << "Element " << i << " mismatch [ \"" << *g << "\" != \"" << *e << "\" ]";
            return res;
        }
    }

    return true;
}

namespace std
{
std::ostream& operator<<(std::ostream& os, const decltype(str2ip("")) &t)
{
    os << "(";
    os << (long)std::get<0>(t) << ", ";
    os << (long)std::get<1>(t) << ", ";
    os << (long)std::get<2>(t) << ", ";
    os << (long)std::get<3>(t);
    return os << ")";
}
}

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_version )
{
    BOOST_CHECK_GT( version(), 0 );
}

BOOST_AUTO_TEST_CASE( test_split )
{
    BOOST_CHECK(compare_collections(split("", "."), {""}));
    BOOST_CHECK(compare_collections(split("11", "."), {"11"}));
    BOOST_CHECK(compare_collections(split("..", "."), {"", "", ""}));
    BOOST_CHECK(compare_collections(split("11.", "."), {"11", ""}));
    BOOST_CHECK(compare_collections(split(".11", "."), {"", "11"}));
    BOOST_CHECK(compare_collections(split("11.22", "."), {"11", "22"}));
}

BOOST_AUTO_TEST_CASE( test_str2ip )
{

    BOOST_CHECK_EQUAL(str2ip("255.255.255.255"), std::make_tuple(255, 255, 255, 255));
    BOOST_CHECK_EQUAL(str2ip("128.128.128.128"), std::make_tuple(128, 128, 128, 128));
    BOOST_CHECK_EQUAL(str2ip("1.2.3.4"), std::make_tuple(1, 2, 3, 4));
    BOOST_CHECK_EQUAL(str2ip("1.2.3.0"), std::make_tuple(1, 2, 3, 0));
    BOOST_CHECK_EQUAL(str2ip("0.2.3.0"), std::make_tuple(0, 2, 3, 0));
    BOOST_CHECK_EQUAL(str2ip("0.2.0.4"), std::make_tuple(0, 2, 0, 4));
    BOOST_CHECK_EQUAL(str2ip("0.0.0.4"), std::make_tuple(0, 0, 0, 4));

    BOOST_CHECK_EQUAL(str2ip("1.2.3.4\t5\t6\n"), std::make_tuple(1, 2, 3, 4));
}

BOOST_AUTO_TEST_CASE( test_ip2str )
{
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(255, 255, 255, 255)), "255.255.255.255");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(128, 128, 128, 128)), "128.128.128.128");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(1, 2, 3, 4)), "1.2.3.4");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(1, 2, 3, 0)), "1.2.3.0");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(0, 2, 3, 0)), "0.2.3.0");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(0, 2, 0, 4)), "0.2.0.4");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(0, 0, 0, 4)), "0.0.0.4");
    BOOST_CHECK_EQUAL(ip2str(std::make_tuple(0, 0, 0, 0)), "0.0.0.0");
}


BOOST_AUTO_TEST_CASE( test_ips_read )
{
    std::stringstream ss;
    ss << "113.162.145.156\t111\t0\n";
    ss << "157.39.22.224\t5\t6\n";
    ss << "79.180.73.190\t2\t1\n";

    BOOST_CHECK(compare_collections(ips_read(ss), {std::make_tuple(113, 162, 145, 156), std::make_tuple(157, 39, 22, 224), std::make_tuple(79, 180, 73, 190)}));
}

BOOST_AUTO_TEST_CASE( test_ips_dump )
{
    decltype(ips_read(std::cin)) ips{std::make_tuple(113, 162, 145, 156), std::make_tuple(157, 39, 22, 224), std::make_tuple(79, 180, 73, 190)};
    std::stringstream ss;
    ips_dump(ss, ips);

    BOOST_CHECK_EQUAL(ss.str(), "113.162.145.156\n157.39.22.224\n79.180.73.190\n");
}

BOOST_AUTO_TEST_CASE( test_check_seq )
{
    decltype(ips_read(std::cin)) ips{std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0), std::make_tuple(1, 0, 0, 0)};

    decltype(ips_read(std::cin)) ips_filter_1;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_1), [](const decltype(str2ip("")) &a) {
        return check_sequence(a, 1);
    });
    BOOST_CHECK(compare_collections(ips_filter_1, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0), std::make_tuple(1, 0, 0, 0)}));

    decltype(ips_read(std::cin)) ips_filter_2;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_2), [](const decltype(str2ip("")) &a) {
        return check_sequence(a, 1, 2);
    });
    BOOST_CHECK(compare_collections(ips_filter_2, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0)}));

    decltype(ips_read(std::cin)) ips_filter_3;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_3), [](const decltype(str2ip("")) &a) {
        return check_sequence(a, 1, 2, 3);
    });
    BOOST_CHECK(compare_collections(ips_filter_3, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0)}));

    decltype(ips_read(std::cin)) ips_filter_4;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_4), [](const decltype(str2ip("")) &a) {
        return check_sequence(a, 1, 2, 3, 4);
    });
    BOOST_CHECK(compare_collections(ips_filter_4, {std::make_tuple(1, 2, 3, 4)}));
}

BOOST_AUTO_TEST_CASE( test_op_or )
{
    BOOST_CHECK_EQUAL(op_or(1==1), true);
    BOOST_CHECK_EQUAL(op_or(1==2), false);
    BOOST_CHECK_EQUAL(op_or(1==2, 2==2), true);
    BOOST_CHECK_EQUAL(op_or(1==2, 2==3), false);
    BOOST_CHECK_EQUAL(op_or(1==2, 2==3, 3==3), true);
    BOOST_CHECK_EQUAL(op_or(1==2, 2==3, 3==4), false);
}

BOOST_AUTO_TEST_CASE( test_check_any )
{
    decltype(ips_read(std::cin)) ips{std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0), std::make_tuple(1, 0, 0, 0)};

    decltype(ips_read(std::cin)) ips_filter_1;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_1), [](const decltype(str2ip("")) &a) {
        return check_any(a, 1);
    });
    BOOST_CHECK(compare_collections(ips_filter_1, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0), std::make_tuple(1, 0, 0, 0)}));

    decltype(ips_read(std::cin)) ips_filter_2;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_2), [](const decltype(str2ip("")) &a) {
        return check_any(a, 2);
    });
    BOOST_CHECK(compare_collections(ips_filter_2, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0), std::make_tuple(1, 2, 0, 0)}));

    decltype(ips_read(std::cin)) ips_filter_3;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_3), [](const decltype(str2ip("")) &a) {
        return check_any(a, 3);
    });
    BOOST_CHECK(compare_collections(ips_filter_3, {std::make_tuple(1, 2, 3, 4), std::make_tuple(1, 2, 3, 0)}));

    decltype(ips_read(std::cin)) ips_filter_4;
    std::copy_if(std::begin(ips), std::end(ips), std::back_inserter(ips_filter_4), [](const decltype(str2ip("")) &a) {
        return check_any(a, 4);
    });
    BOOST_CHECK(compare_collections(ips_filter_4, {std::make_tuple(1, 2, 3, 4)}));
}


BOOST_AUTO_TEST_SUITE_END()