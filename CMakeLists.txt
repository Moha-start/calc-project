cmake_minimum_required(VERSION 3.5)
project(calculator C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(Stack SHARED stack.c)

add_library(Calc SHARED calc.c)
target_link_libraries(Calc Stack m)

add_executable(calculator main.c)
target_link_libraries(calculator Calc m)

add_executable(stack stack_main.c)
target_link_libraries(stack Stack)

# -----------------------------------------------------
# Unit testing executable
# -----------------------------------------------------
add_executable(calc_test calc_test.c)
target_link_libraries(calc_test Calc Stack m)


# -----------------------------------------------------
# Standard Tests
# -----------------------------------------------------
add_test(test_calc_dummy ./calc_test dummy)
add_test(test_calc_getCalc ./calc_test getCalc)
add_test(test_calc_IsSameOpen ./calc_test IsSameOpen)
add_test(test_calc_FreeCalc ./calc_test FreeCalc)
add_test(test_calc_AreDelimitersValid ./calc_test AreDelimitersValid)
add_test(test_calc_countOpeningDelimiters ./calc_test countOpeningDelimiters)
add_test(test_calc_printCalc ./calc_test printCalc)
add_test(test_calc_calctype ./calc_test calctype)
add_test(test_calc_getCalctype ./calc_test getCalctype)
add_test(test_calc_sizeCalc ./calc_test sizeCalc)
add_test(test_calc_moveResult ./calc_test moveResult)
add_test(test_calc_fromStringToDouble ./calc_test fromStringToDouble)
add_test(test_calc_fromDoobleToString ./calc_test fromDoobleToString)
add_test(test_calc_isMixOperation ./calc_test isMixOperation)
add_test(test_calc_calculateSimple ./calc_test calculateSimple)
add_test(test_calc_isPrio ./calc_test isPrio)
add_test(test_calc_solveSimple ./calc_test solveSimple)
add_test(test_calc_evaluateParenthesizedExpression ./calc_test evaluateParenthesizedExpression)
add_test(test_calc_solve ./calc_test solve)

# -----------------------------------------------------
# Memory Tests
# -----------------------------------------------------
find_program(VALGRIND_COMMAND valgrind)
if(VALGRIND_COMMAND)
    add_test(test_mem_dummy ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test dummy)
    add_test(test_mem_getCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getCalc)
    add_test(test_mem_IsSameOpen ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test IsSameOpen)
    add_test(test_mem_FreeCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test FreeCalc)
    add_test(test_mem_AreDelimitersValid ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test AreDelimitersValid)
    add_test(test_mem_countOpeningDelimiters ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test countOpeningDelimiters)
    add_test(test_mem_printCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test printCalc)
    add_test(test_mem_calctype ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test calctype)
    add_test(test_mem_getCalctype ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test getCalctype)
    add_test(test_mem_sizeCalc ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test sizeCalc)
    add_test(test_mem_moveResult ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test moveResult)
    add_test(test_mem_fromStringToDouble ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test fromStringToDouble)
    add_test(test_mem_fromDoobleToString ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test fromDoobleToString)
    add_test(test_mem_isMixOperation ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test isMixOperation)
    add_test(test_mem_calculateSimple ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test calculateSimple)
    add_test(test_mem_isPrio ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test isPrio)
    add_test(test_mem_solveSimple ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test solveSimple)
    add_test(test_mem_evaluateParenthesizedExpression ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test evaluateParenthesizedExpression)
    add_test(test_mem_solve ${VALGRIND_COMMAND} --leak-check=full --error-exitcode=1 ./calc_test solve)
endif()
