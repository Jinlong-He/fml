#include "test.hpp"
#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
using namespace boost;

BOOST_AUTO_TEST_SUITE(test_fa)

BOOST_AUTO_TEST_CASE(test_minimize1) {
    BOOST_CHECK_EQUAL(test::test_minimize1(), true);
}

BOOST_AUTO_TEST_CASE(test_minimize2) {
    BOOST_CHECK_EQUAL(test::test_minimize2(), true);
}

BOOST_AUTO_TEST_CASE(test_minimize3) {
    BOOST_CHECK_EQUAL(test::test_minimize3(), true);
}

BOOST_AUTO_TEST_CASE(test_minimize4) {
    BOOST_CHECK_EQUAL(test::test_minimize4(), true);
}

BOOST_AUTO_TEST_CASE(test_minimize5) {
    BOOST_CHECK_EQUAL(test::test_minimize5(), true);
}

BOOST_AUTO_TEST_CASE(test_basic_regular_expression1) {
    BOOST_CHECK_EQUAL(test::test_basic_regular_expression1(), true);
}

BOOST_AUTO_TEST_CASE(test_basic_regular_expression2) {
    BOOST_CHECK_EQUAL(test::test_basic_regular_expression2(), true);
}

BOOST_AUTO_TEST_CASE(test_basic_regular_expression3) {
    BOOST_CHECK_EQUAL(test::test_basic_regular_expression3(), true);
}

BOOST_AUTO_TEST_CASE(test_basic_regular_expression4) {
    BOOST_CHECK_EQUAL(test::test_basic_regular_expression4(), true);
}

BOOST_AUTO_TEST_SUITE_END()

