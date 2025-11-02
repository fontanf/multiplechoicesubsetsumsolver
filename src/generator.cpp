#include "multiplechoicesubsetsumsolver/generator.hpp"

#include "multiplechoicesubsetsumsolver/instance_builder.hpp"

using namespace multiplechoicesubsetsumsolver;

Instance multiplechoicesubsetsumsolver::generate_random(
        GroupId number_of_groups,
        ItemId number_of_items_in_groups,
        Weight data_range,
        double capacity_ratio,
        std::mt19937_64& generator)
{
    InstanceBuilder instance_builder;

    // Add items.
    Weight weight_max = 0;
    Weight weight_sum = 0;
    std::uniform_int_distribution<Weight> distribution(1, data_range);
    for (GroupId group_id = 0; group_id < number_of_groups; ++group_id) {
        Weight group_weight_max = 0;
        for (ItemId pos = 0; pos < number_of_items_in_groups; ++pos) {
            Weight weight = distribution(generator);
            instance_builder.add_item(group_id, weight);

            weight_max = std::max(weight_max, weight);
            group_weight_max = std::max(group_weight_max, weight);
        }
        weight_sum += group_weight_max;
    }

    // Compute capacity.
    Weight capacity = std::max(
            weight_max,
            (Weight)(capacity_ratio * weight_sum));
    instance_builder.set_capacity(capacity);

    return instance_builder.build();
}
