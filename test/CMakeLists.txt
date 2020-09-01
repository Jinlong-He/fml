ADD_LIBRARY (test_minimize test_minimize.cpp)
ADD_LIBRARY (test_bre test_basic_regular_expression.cpp)
ADD_LIBRARY (test_intersect test_intersect.cpp)
ADD_LIBRARY (test_union test_union.cpp)

ADD_EXECUTABLE (
    main main.cpp 
    test_minimize.cpp
    test_intersect.cpp
    test_union.cpp
    test_basic_regular_expression.cpp 
    test_fomula_automaton.cpp
    #test_transducer.cpp
    #test_template.cpp 
    #test_state_prop.cpp 
    #test_poststar.cpp
    #test_propositional_logic.cpp
    #test_ctl.cpp
    #test_ltl.cpp
    )

FIND_PACKAGE (Boost 1.54 REQUIRED COMPONENTS unit_test_framework)

ADD_EXECUTABLE (test_fml test.cpp)

TARGET_LINK_LIBRARIES (test_fml
  PRIVATE
      test_minimize
      test_bre
      test_intersect
      test_union
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
