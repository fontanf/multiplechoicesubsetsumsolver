#include "tests.hpp"
#include "multiplechoicesubsetsumsolver/algorithms/dynamic_programming_bellman.hpp"
#include "multiplechoicesubsetsumsolver/instance_builder.hpp"

#include <boost/filesystem.hpp>

using namespace multiplechoicesubsetsumsolver;
namespace fs = boost::filesystem;

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

struct MultipleChoiceSubsetSumDynamicProgrammingBellmanTestParams
{
    fs::path instance_path;

    fs::path certificate_path;
};

inline std::ostream& operator<<(
        std::ostream& os,
        const MultipleChoiceSubsetSumDynamicProgrammingBellmanTestParams& test_params)
{
    os << test_params.instance_path;
    return os;
}

class MultipleChoiceSubsetSumDynamicProgrammingBellmanTest: public testing::TestWithParam<MultipleChoiceSubsetSumDynamicProgrammingBellmanTestParams> { };

TEST_P(MultipleChoiceSubsetSumDynamicProgrammingBellmanTest, MultipleChoiceSubsetSumDynamicProgrammingBellman)
{
    MultipleChoiceSubsetSumDynamicProgrammingBellmanTestParams test_params = GetParam();
    InstanceBuilder instance_builder;
    instance_builder.read(test_params.instance_path.string(), "standard");
    const Instance instance = instance_builder.build();
    const Solution solution(instance, test_params.certificate_path.string());

    auto output_array = dynamic_programming_bellman_array(instance);
    EXPECT_EQ(output_array.value, solution.weight());
    EXPECT_EQ(output_array.value, output_array.solution.weight());
    EXPECT_EQ(output_array.bound, solution.weight());

    auto output_word_ram = dynamic_programming_bellman_word_ram(instance);
    EXPECT_EQ(output_word_ram.value, solution.weight());
    EXPECT_EQ(output_word_ram.bound, solution.weight());

    auto output_word_ram_rec = dynamic_programming_bellman_word_ram_rec(instance);
    EXPECT_EQ(output_word_ram_rec.value, solution.weight());
    EXPECT_EQ(output_word_ram_rec.value, output_word_ram_rec.solution.weight());
    EXPECT_EQ(output_word_ram_rec.bound, solution.weight());
}

INSTANTIATE_TEST_SUITE_P(
        MultipleChoiceSubsetSum,
        MultipleChoiceSubsetSumDynamicProgrammingBellmanTest,
        testing::ValuesIn(std::vector<MultipleChoiceSubsetSumDynamicProgrammingBellmanTestParams>{
            {
                fs::path("data") / "tests" / "no_group" / "instance.txt",
                fs::path("data") / "tests" / "no_group" / "solution.txt",
            }, {
                fs::path("data") / "tests" / "single_group" / "instance.txt",
                fs::path("data") / "tests" / "single_group" / "solution.txt",
            }, {
                fs::path("data") / "tests" / "single_group_too_heavy" / "instance.txt",
                fs::path("data") / "tests" / "single_group_too_heavy" / "solution.txt",
            }}));
