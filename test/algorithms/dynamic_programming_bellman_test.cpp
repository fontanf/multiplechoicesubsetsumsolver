#include "tests.hpp"
#include "multiplechoicesubsetsumsolver/algorithms/dynamic_programming_bellman.hpp"

using namespace multiplechoicesubsetsumsolver;

TEST_P(ExactAlgorithmTest, ExactAlgorithm)
{
    TestParams test_params = GetParam();
    const Instance instance = get_instance(test_params.files);
    const Solution solution = get_solution(instance, test_params.files);
    auto output = test_params.algorithm(instance);
    EXPECT_EQ(output.value, solution.weight());
    EXPECT_EQ(output.value, output.solution.weight());
    EXPECT_EQ(output.bound, solution.weight());
}

TEST_P(ExactNoSolutionAlgorithmTest, ExactNoSolutionAlgorithm)
{
    TestParams test_params = GetParam();
    const Instance instance = get_instance(test_params.files);
    const Solution solution = get_solution(instance, test_params.files);
    auto output = test_params.algorithm(instance);
    EXPECT_EQ(output.value, solution.weight());
    EXPECT_EQ(output.bound, solution.weight());
}

INSTANTIATE_TEST_SUITE_P(
        MultipleChoiceKnapsackDynamicProgrammingBellmanArray,
        ExactAlgorithmTest,
        testing::ValuesIn(get_test_params(
                {
                    [](const Instance& instance)
                    {
                        return dynamic_programming_bellman_array(instance);
                    },
                },
                {
                    get_test_instance_paths(get_path({"data", "test_dynamic_programming_bellman_array.txt"})),
                })));

INSTANTIATE_TEST_SUITE_P(
        MultipleChoiceKnapsackDynamicProgrammingBellmanWordRam,
        ExactNoSolutionAlgorithmTest,
        testing::ValuesIn(get_test_params(
                {
                    [](const Instance& instance)
                    {
                        return dynamic_programming_bellman_word_ram(instance);
                    },
                },
                {
                    get_test_instance_paths(get_path({"data", "test_dynamic_programming_bellman_word_ram_rec.txt"})),
                })));

INSTANTIATE_TEST_SUITE_P(
        MultipleChoiceKnapsackDynamicProgrammingBellmanWordRamRec,
        ExactAlgorithmTest,
        testing::ValuesIn(get_test_params(
                {
                    [](const Instance& instance)
                    {
                        return dynamic_programming_bellman_word_ram_rec(instance);
                    },
                },
                {
                    get_test_instance_paths(get_path({"data", "test_dynamic_programming_bellman_word_ram_rec.txt"})),
                })));
