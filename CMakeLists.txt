cmake_minimum_required(VERSION 3.10)
project(calculator C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(Stack SHARED stack.c)

# Updated to include the newly split calculation source files alongside math.c
add_library(Calc SHARED calc_core.c calc_parser.c calc_eval.c math.c)
target_link_libraries(Calc Stack)

add_executable(calculator main.c)
target_link_libraries(calculator Calc)

add_executable(stack stack_main.c)
target_link_libraries(stack Stack)

# -----------------------------------------------------
# Unit testing executable
# -----------------------------------------------------
add_executable(calc_test calc_test.c)
target_link_libraries(calc_test Calc Stack)

# -----------------------------------------------------
# Standard Tests
# -----------------------------------------------------
add_test(test_calc_dummy ./calc_test dummy)
add_test(test_calc_getCalc ./calc_test getCalc)
add_test(test_calc_isMatchingDelimiter ./calc_test isMatchingDelimiter)
add_test(test_calc_freeCalc ./calc_test freeCalc)
add_test(test_calc_areDelimitersValid ./calc_test areDelimitersValid)
add_test(test_calc_countOpeningDelimiters ./calc_test countOpeningDelimiters)
add_test(test_calc_printCalc ./calc_test printCalc)
add_test(test_calc_getCharType ./calc_test getCharType)
add_test(test_calc_getTokenType ./calc_test getTokenType)
add_test(test_calc_getCalcSize ./calc_test getCalcSize)
add_test(test_calc_replaceRangeWithResult ./calc_test replaceRangeWithResult)
add_test(test_calc_stringToDouble ./calc_test stringToDouble)
add_test(test_calc_doubleToString ./calc_test doubleToString)
add_test(test_calc_hasMixedOperations ./calc_test hasMixedOperations)
add_test(test_calc_applyBinaryOperator ./calc_test applyBinaryOperator)
add_test(test_calc_findHighestPrecedenceOp ./calc_test findHighestPrecedenceOp)
add_test(test_calc_solveFlatExpression ./calc_test solveFlatExpression)
add_test(test_calc_evalDelimitedExpression ./calc_test evalDelimitedExpression)
add_test(test_calc_solve ./calc_test solve)

# -----------------------------------------------------
# Memory Tests
# -----------------------------------------------------
find_program(VALGRIND_COMMAND valgrind)
if(VALGRIND_COMMAND)
    add_test(test_mem_dummy ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test dummy)
    add_test(test_mem_getCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getCalc)
    add_test(test_mem_isMatchingDelimiter ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test isMatchingDelimiter)
    add_test(test_mem_freeCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test freeCalc)
    add_test(test_mem_areDelimitersValid ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test areDelimitersValid)
    add_test(test_mem_countOpeningDelimiters ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test countOpeningDelimiters)
    add_test(test_mem_printCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test printCalc)
    add_test(test_mem_getCharType ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getCharType)
    add_test(test_mem_getTokenType ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getTokenType)
    add_test(test_mem_getCalcSize ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getCalcSize)
    add_test(test_mem_replaceRangeWithResult ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test replaceRangeWithResult)
    add_test(test_mem_stringToDouble ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test stringToDouble)
    add_test(test_mem_doubleToString ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test doubleToString)
    add_test(test_mem_hasMixedOperations ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test hasMixedOperations)
    add_test(test_mem_applyBinaryOperator ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test applyBinaryOperator)
    add_test(test_mem_findHighestPrecedenceOp ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test findHighestPrecedenceOp)
    add_test(test_mem_solveFlatExpression ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test solveFlatExpression)
    add_test(test_mem_evalDelimitedExpression ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test evalDelimitedExpression)
    add_test(test_mem_solve ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test solve)
endif()

foreach(t
    trimTrailingZeros isUnaryOperation combineDigits
    getOperatorPrecedence hasHigherPrecedence evalUnaryOperation
    solveUnaryOperation evalUnaryOperations solveString
    power mod computeTrig stack
)
    add_test(NAME test_calc_${t} COMMAND ./calc_test ${t})
    if(VALGRIND_COMMAND)
        add_test(NAME test_mem_${t} COMMAND ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test ${t})
    endif()
endforeach()