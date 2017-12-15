#include <spdlog/spdlog.h>

#define BOOST_TEST_MODULE TestMain
#include <boost/test/unit_test.hpp>

#include "../bin/version.h"
#include "tools.h"

#include <iostream>
#include <boost/type_index.hpp>

template<typename T>
boost::test_tools::predicate_result compare_collections(const T &got, const T &expect)
{
  if( got.size() != expect.size() )
  {
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

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_version )
{
    BOOST_CHECK_GT( version(), 0 );
}

BOOST_AUTO_TEST_CASE( test_str2ip )
{
    BOOST_CHECK_EQUAL(str2ip("255.255.255.255"), 0xffffffff);
    BOOST_CHECK_EQUAL(str2ip("128.128.128.128"), 0x80808080);
    BOOST_CHECK_EQUAL(str2ip("1.2.3.4"), 0x01020304);
    BOOST_CHECK_EQUAL(str2ip("1.2.3.0"), 0x01020300);
    BOOST_CHECK_EQUAL(str2ip("0.2.3.0"), 0x00020300);
    BOOST_CHECK_EQUAL(str2ip("0.2.0.4"), 0x00020004);
    BOOST_CHECK_EQUAL(str2ip("0.0.0.4"), 0x00000004);

    BOOST_CHECK_EQUAL(str2ip("1.2.3.4\t5\t6\n"), 0x01020304);
    BOOST_CHECK_EQUAL(str2ip("1.2.3.\t5\t6\n"), 0x01020300);
    BOOST_CHECK_EQUAL(str2ip("1.2.3."), 0x01020300);
    BOOST_CHECK_EQUAL(str2ip("1.2.3"), 0x01020300);
    BOOST_CHECK_EQUAL(str2ip("1.2."), 0x01020000);
    BOOST_CHECK_EQUAL(str2ip("1.2"), 0x01020000);
    BOOST_CHECK_EQUAL(str2ip("1."), 0x01000000);
    BOOST_CHECK_EQUAL(str2ip("1"), 0x01000000);
    BOOST_CHECK_EQUAL(str2ip(""), 0x00000000);
}

BOOST_AUTO_TEST_CASE( test_ip2str )
{
    BOOST_CHECK_EQUAL(ip2str(0xffffffff), "255.255.255.255");
    BOOST_CHECK_EQUAL(ip2str(0x80808080), "128.128.128.128");
    BOOST_CHECK_EQUAL(ip2str(0x01020304), "1.2.3.4");
    BOOST_CHECK_EQUAL(ip2str(0x01020300), "1.2.3.0");
    BOOST_CHECK_EQUAL(ip2str(0x00020300), "0.2.3.0");
    BOOST_CHECK_EQUAL(ip2str(0x00020004), "0.2.0.4");
    BOOST_CHECK_EQUAL(ip2str(0x00000004), "0.0.0.4");
    BOOST_CHECK_EQUAL(ip2str(0x00000000), "0.0.0.0");
}

BOOST_AUTO_TEST_CASE( test_ips_read )
{
    std::stringstream ss;
    ss << "113.162.145.156\t111\t0\n";
    ss << "157.39.22.224\t5\t6\n";
    ss << "79.180.73.190\t2\t1\n";

    BOOST_TEST(compare_collections(ips_read(ss), {0x71a2919c, 0x9d2716e0, 0x4fb449be}));
}

BOOST_AUTO_TEST_CASE( test_ips_dump )
{
    std::vector<uint32_t> ips{0x71a2919c, 0x9d2716e0, 0x4fb449be};
    std::stringstream ss;
    ips_dump(ss, ips);

    BOOST_CHECK_EQUAL(ss.str(), "113.162.145.156\n157.39.22.224\n79.180.73.190\n");
}

BOOST_AUTO_TEST_SUITE_END()