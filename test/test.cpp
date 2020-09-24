#include "test.hpp"
#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <fstream>
using namespace boost;

BOOST_AUTO_TEST_SUITE(test_fa)

BOOST_AUTO_TEST_CASE(test_clear1) {
    BOOST_CHECK_EQUAL(test::test_clear1(), true);
}

BOOST_AUTO_TEST_CASE(test_clear2) {
    BOOST_CHECK_EQUAL(test::test_clear2(), true);
}

BOOST_AUTO_TEST_CASE(test_clear3) {
    BOOST_CHECK_EQUAL(test::test_clear3(), true);
}

BOOST_AUTO_TEST_CASE(test_clear4) {
    BOOST_CHECK_EQUAL(test::test_clear4(), true);
}

BOOST_AUTO_TEST_CASE(test_clear5) {
    BOOST_CHECK_EQUAL(test::test_clear5(), true);
}

BOOST_AUTO_TEST_CASE(test_clear6) {
    BOOST_CHECK_EQUAL(test::test_clear6(), true);
}

BOOST_AUTO_TEST_CASE(test_clear7) {
    BOOST_CHECK_EQUAL(test::test_clear7(), true);
}

BOOST_AUTO_TEST_CASE(test_clear8) {
    BOOST_CHECK_EQUAL(test::test_clear8(), true);
}

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

BOOST_AUTO_TEST_CASE(test_minimize8) {
    BOOST_CHECK_EQUAL(test::test_minimize8(), true);
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

BOOST_AUTO_TEST_CASE(test_intersect5) {
    BOOST_CHECK_EQUAL(test::test_intersect5(), true);
}

BOOST_AUTO_TEST_CASE(test_union1) {
    BOOST_CHECK_EQUAL(test::test_union1(), true);
}

BOOST_AUTO_TEST_CASE(test_union2) {
    BOOST_CHECK_EQUAL(test::test_union2(), true);
}

BOOST_AUTO_TEST_CASE(test_union3) {
    BOOST_CHECK_EQUAL(test::test_union3(), true);
}

BOOST_AUTO_TEST_CASE(test_union4) {
    BOOST_CHECK_EQUAL(test::test_union4(), true);
}

BOOST_AUTO_TEST_CASE(test_union5) {
    BOOST_CHECK_EQUAL(test::test_union5(), true);
}

BOOST_AUTO_TEST_CASE(test_concat1) {
    BOOST_CHECK_EQUAL(test::test_concat1(), true);
}

BOOST_AUTO_TEST_CASE(test_concat2) {
    BOOST_CHECK_EQUAL(test::test_concat2(), true);
}

BOOST_AUTO_TEST_CASE(test_concat3) {
    BOOST_CHECK_EQUAL(test::test_concat3(), true);
}

BOOST_AUTO_TEST_CASE(test_subset1) {
    BOOST_CHECK_EQUAL(test::test_subset1(), true);
}

BOOST_AUTO_TEST_CASE(test_subset2) {
    BOOST_CHECK_EQUAL(test::test_subset2(), true);
}

BOOST_AUTO_TEST_CASE(test_composite1) {
    BOOST_CHECK_EQUAL(test::test_composite1(), true);
}

BOOST_AUTO_TEST_CASE(test_poststar) {
    BOOST_CHECK_EQUAL(test::test_poststar(), true);
}

BOOST_AUTO_TEST_CASE(test_prestar) {
    BOOST_CHECK_EQUAL(test::test_prestar(), true);
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

