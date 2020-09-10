#include "test.hpp"
#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <fstream>
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

BOOST_AUTO_TEST_CASE(test_minimize6) {
    BOOST_CHECK_EQUAL(test::test_minimize6(), true);
}

BOOST_AUTO_TEST_CASE(test_minimize7) {
    BOOST_CHECK_EQUAL(test::test_minimize7(), true);
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

BOOST_AUTO_TEST_CASE(test_basic_regular_expression5) {
    BOOST_CHECK_EQUAL(test::test_basic_regular_expression5(), true);
}

BOOST_AUTO_TEST_CASE(test_intersect1) {
    BOOST_CHECK_EQUAL(test::test_intersect1(), true);
}

BOOST_AUTO_TEST_CASE(test_intersect2) {
    BOOST_CHECK_EQUAL(test::test_intersect2(), true);
}

BOOST_AUTO_TEST_CASE(test_intersect3) {
    BOOST_CHECK_EQUAL(test::test_intersect3(), true);
}

BOOST_AUTO_TEST_CASE(test_intersect4) {
    BOOST_CHECK_EQUAL(test::test_intersect4(), true);
}

BOOST_AUTO_TEST_CASE(test_union1) {
    BOOST_CHECK_EQUAL(test::test_union1(), true);
}

BOOST_AUTO_TEST_CASE(test_union2) {
    BOOST_CHECK_EQUAL(test::test_union2(), true);
}

BOOST_AUTO_TEST_CASE(test_fomula_automaton) {
    BOOST_CHECK_EQUAL(test::test_fomula_automaton(), true);
}

// test for ll.
BOOST_AUTO_TEST_CASE(test_propositional_logic) {
    BOOST_CHECK_EQUAL(test::test_propositional_logic(), true);
}

BOOST_AUTO_TEST_CASE(test_ltl) {
    BOOST_CHECK_EQUAL(test::test_ltl(), true);
}

BOOST_AUTO_TEST_CASE(test_ctl) {
    BOOST_CHECK_EQUAL(test::test_ctl(), true);
}

BOOST_AUTO_TEST_SUITE_END()

