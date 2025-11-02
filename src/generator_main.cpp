#include "multiplechoicesubsetsumsolver/generator.hpp"

#include <boost/program_options.hpp>

using namespace multiplechoicesubsetsumsolver;

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    GroupId number_of_groups = 10;
    ItemId number_of_items_in_groups = 10;
    Weight data_range = 1000;
    double capacity_ratio = 0.5;
    Seed seed = 0;
    std::string output_path = "";

    // Parse program options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("number-of-groups,", po::value<GroupId>(&number_of_groups)->required(), "set number of groups")
        ("number-of-items-in-groups,", po::value<ItemId>(&number_of_items_in_groups)->required(), "set number of items in groups")
        ("data-range,", po::value<Weight>(&data_range)->required(), "set data range")
        ("capacity-ratio,", po::value<double>(&capacity_ratio)->required(), "set capacity ratio")
        ("seed,", po::value<Seed>(&seed)->required(), "set seed")
        ("output,", po::value<std::string>(&output_path)->required(), "set output path")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (const po::required_option& e) {
        std::cout << desc << std::endl;;
        return 1;
    }

    std::mt19937_64 generator(seed);
    Instance instance = generate_random(
            number_of_groups,
            number_of_items_in_groups,
            data_range,
            capacity_ratio,
            generator);
    instance.write(output_path);

    return 0;
}
