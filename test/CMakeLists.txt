# ADD_LIBRARY (test_clear test_clear.cpp)
# ADD_LIBRARY (test_minimize test_minimize.cpp)
# ADD_LIBRARY (test_bre test_basic_regular_expression.cpp)
# ADD_LIBRARY (test_intersect test_intersect.cpp)
# ADD_LIBRARY (test_union test_union.cpp)
# ADD_LIBRARY (test_concat test_concat.cpp)
# ADD_LIBRARY (test_subset test_subset.cpp)
# ADD_LIBRARY (test_poststar test_poststar.cpp)
# ADD_LIBRARY (test_prestar test_prestar.cpp)
# ADD_LIBRARY (test_composite test_composite.cpp)
# ADD_LIBRARY (test_left_quotient test_left_quotient.cpp)
# ADD_LIBRARY (test_fomula_automaton test_fomula_automaton.cpp)
# ADD_LIBRARY (test_propositional_logic test_propositional_logic.cpp)
# ADD_LIBRARY (test_ltl test_ltl.cpp)
# ADD_LIBRARY (test_ctl test_ctl.cpp)
# ADD_LIBRARY (test_util test_util.cpp)
# ADD_LIBRARY (test_product test_product.cpp)
ADD_LIBRARY (test_sra test_sra.cpp)

ADD_EXECUTABLE (main main.cpp)
TARGET_LINK_LIBRARIES (main
  PRIVATE
      # test_clear
      # test_minimize
      # test_bre
      # test_intersect
      # test_union
      # test_concat
      # test_subset
      # test_poststar
      # test_prestar
      # test_composite
      # test_left_quotient
      # test_fomula_automaton
      # test_propositional_logic
      # test_ltl
      # test_ctl
      # test_util
      # test_product
      test_sra
  )


FIND_PACKAGE (Boost 1.54 REQUIRED COMPONENTS unit_test_framework)

ADD_EXECUTABLE (test_fml test.cpp)

TARGET_LINK_LIBRARIES (test_fml
  PRIVATE
      # test_clear
      # test_minimize
      # test_bre
      # test_intersect
      # test_union
      # test_concat
      # test_subset
      # test_poststar
      # test_prestar
      # test_composite
      # test_left_quotient
      # test_fomula_automaton
      # test_propositional_logic
      # test_ltl
      # test_ctl
      # test_util
      # test_product
      test_sra
      Boost::unit_test_framework
  )

# avoid undefined reference to "main" in test.cpp
TARGET_COMPILE_DEFINITIONS (test_fml
  PRIVATE
      BOOST_TEST_DYN_LINK
  )

ENABLE_TESTING()

ADD_TEST (
  NAME boost_test
  COMMAND $<TARGET_FILE:test_fml>
  )
