#include <spdlog/spdlog.h>

#define BOOST_TEST_MODULE TestMain
#include <boost/test/unit_test.hpp>

#include "../bin/version.h"
#include "tools.h"

#include <iostream>
#include <boost/type_index.hpp>

template<typename gotC, typename expectC = gotC>
boost::test_tools::predicate_result compare_collections(const gotC &got, const expectC &expect)
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

auto console = spdlog::stdout_logger_st("console");

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_version )
{
    console->info("test version");

    BOOST_CHECK_GT( version(), 0 );
}

BOOST_AUTO_TEST_CASE( test_split )
{
    console->info("test split");

    BOOST_TEST(compare_collections(split("", '.'), {""}));
    BOOST_TEST(compare_collections(split("11", '.'), {"11"}));
    BOOST_TEST(compare_collections(split("..", '.'), {"","",""}));
    BOOST_TEST(compare_collections(split(".11", '.'), {"","11"}));
    BOOST_TEST(compare_collections(split("11.22", '.'), {"11","22"}));

    BOOST_TEST(compare_collections(split("11.22\t33", ".\t"), {"11","22","33"}));
}

// BOOST_AUTO_TEST_CASE( test_splice )
// {
//     console->info("test splice");

//     BOOST_TEST(compare_collections(split("", '.'), {""}));
//     BOOST_TEST(compare_collections(split("11", '.'), {"11"}));
//     BOOST_TEST(compare_collections(split("..", '.'), {"","",""}));
//     BOOST_TEST(compare_collections(split(".11", '.'), {"","11"}));
//     BOOST_TEST(compare_collections(split("11.22", '.'), {"11","22"}));

//     BOOST_TEST(compare_collections(split("11.22\t33", ".\t"), {"11","22","33"}));
// }

BOOST_AUTO_TEST_CASE( test_join )
{
    console->info("test join()");

    BOOST_TEST(join< std::initializer_list<std::string> >({"1"}, '.') == "1");
    BOOST_TEST(join({"1", "2"}, '.') == "1.2");
    BOOST_TEST(join({"1", "2", "3"}, "..") == "1..2..3");
    BOOST_TEST(join({"1", "2", "3", "4", "5", "6"}, '.', 4) == "1.2.3.4");
    // BOOST_TEST(join< std::initializer_list<int> >({1, 2, 3, 4, 5, 6}, '.', 4) == "1.2.3.4");
}

BOOST_AUTO_TEST_SUITE_END()