add_library(MultipleChoiceSubsetSumSolver_tests)
target_sources(MultipleChoiceSubsetSumSolver_tests PRIVATE
    tests.cpp)
target_link_libraries(MultipleChoiceSubsetSumSolver_tests PUBLIC
    MultipleChoiceSubsetSumSolver
    Boost::filesystem
    GTest::gtest_main)
add_library(MultipleChoiceSubsetSumSolver::tests ALIAS MultipleChoiceSubsetSumSolver_tests)

add_executable(MultipleChoiceSubsetSumSolver_dynamic_programming_bellman_test)
target_sources(MultipleChoiceSubsetSumSolver_dynamic_programming_bellman_test PRIVATE
    dynamic_programming_bellman_test.cpp)
target_link_libraries(MultipleChoiceSubsetSumSolver_dynamic_programming_bellman_test
    MultipleChoiceSubsetSumSolver_tests
    MultipleChoiceSubsetSumSolver_dynamic_programming_bellman
    GTest::gtest_main)
gtest_discover_tests(MultipleChoiceSubsetSumSolver_dynamic_programming_bellman_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES TIMEOUT 3600)
