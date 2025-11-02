#pragma once

#include "multiplechoicesubsetsumsolver/instance.hpp"

#include <random>

namespace multiplechoicesubsetsumsolver
{

Instance generate_random(
        GroupId number_of_groups,
        ItemId number_of_items_in_groups,
        Weight data_range,
        double capacity_ratio,
        std::mt19937_64& generator);

}
