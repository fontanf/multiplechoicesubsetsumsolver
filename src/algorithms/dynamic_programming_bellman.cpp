#include "multiplechoicesubsetsumsolver/algorithms/dynamic_programming_bellman.hpp"

#include "multiplechoicesubsetsumsolver/algorithm_formatter.hpp"

using namespace multiplechoicesubsetsumsolver;

namespace
{

/**
 * Compute the greatest common divisor of the weights of the items.
 *
 * The weight of every subset of items is a multiple of it: dividing the
 * weights and the capacity (rounded down) by it doesn't change the solutions,
 * and divides the tables of the dynamic programming algorithms by it.
 */
Weight compute_weights_gcd(const Instance& instance)
{
    Weight gcd = 0;
    for (ItemId item_id = 0; item_id < instance.number_of_items(); ++item_id) {
        Weight weight = instance.item(item_id).weight;
        while (weight != 0) {
            Weight remainder = gcd % weight;
            gcd = weight;
            weight = remainder;
        }
    }
    // No item, or only items of weight 0.
    return (gcd > 0)? gcd: 1;
}

}

////////////////////////////////////////////////////////////////////////////////
////////////////////// dynamic_programming_bellman_array ///////////////////////
////////////////////////////////////////////////////////////////////////////////

Output multiplechoicesubsetsumsolver::dynamic_programming_bellman_array(
        const Instance& instance,
        const Parameters& parameters)
{
    Output output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Dynamic programming - Bellman - array");
    algorithm_formatter.print_header();

    Weight gcd = compute_weights_gcd(instance);
    Weight capacity = instance.capacity() / gcd;
    std::vector<ItemId> values_prev(capacity + 1, -1);
    std::vector<ItemId> values_next(capacity + 1, -1);
    values_next[0] = -2;
    Weight weight_sum = 0;
    // Divided weights of the items of the current group.
    std::vector<Weight> group_weights;
    for (GroupId group_id = 0; group_id < instance.number_of_groups(); ++group_id) {
        // Check time
        if (parameters.timer.needs_to_end()) {
            algorithm_formatter.end();
            return output;
        }

        // Update DP table
        values_prev = values_next;
        const std::vector<ItemId>& item_ids = instance.group(group_id).item_ids;
        group_weights.clear();
        Weight weight_max = 0;
        Weight weight_min = capacity;
        for (ItemId item_id: item_ids) {
            Weight item_weight = instance.item(item_id).weight / gcd;
            group_weights.push_back(item_weight);
            weight_max = std::max(weight_max, item_weight);
            weight_min = std::min(weight_min, item_weight);
        }
        weight_sum += weight_max;
        for (Weight weight = std::min(capacity, weight_sum);
                weight >= weight_min;
                weight--) {
            if (values_prev[weight] != -1)
                continue;
            for (ItemId item_pos = 0; item_pos < (ItemId)item_ids.size(); ++item_pos) {
                if (weight - group_weights[item_pos] >= 0
                        && values_prev[weight - group_weights[item_pos]] != -1) {
                    values_next[weight] = item_ids[item_pos];
                    break;
                }
            }
        }

        //for (Weight w = 0; w < c; ++w)
        //    std::cout << (values[w] >= 0);
        //std::cout << std::endl;

        if (values_next[capacity] != -1)
            break;
    }

    Solution solution(instance);
    Weight weight_cur = 0;
    for (Weight weight = capacity; weight >= 0; weight--) {
        if (values_next[weight] != -1) {
            weight_cur = weight;
            break;
        }
    }
    //std::cout << "retrieve" << std::endl;
    while (weight_cur > 0) {
        ItemId item_id = values_next[weight_cur];
        solution.add(item_id);
        weight_cur -= instance.item(item_id).weight / gcd;
        //std::cout << "j " << j
        //    << " group " << instance.item(j).group_id
        //    << " w " << instance.item(j).weight
        //    << std::endl;
    }

    // Update solution.
    algorithm_formatter.update_solution(
            solution,
            "algorithm end (solution)");
    // Update bound.
    algorithm_formatter.update_bound(
            solution.weight(),
            "algorithm end (bound)");

    algorithm_formatter.end();
    return output;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////// dynamic_programming_bellman_word_ram /////////////////////
////////////////////////////////////////////////////////////////////////////////

Output multiplechoicesubsetsumsolver::dynamic_programming_bellman_word_ram(
        const Instance& instance,
        const Parameters& parameters)
{
    Output output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Dynamic programming - Bellman - word RAM");
    algorithm_formatter.print_header();

    Weight gcd = compute_weights_gcd(instance);
    Weight capacity = instance.capacity() / gcd;
    Weight capacity_number_of_words = (capacity >> 6);
    Weight capacity_number_of_bits_to_shift = capacity - (capacity_number_of_words << 6);
    std::vector<uint64_t> values_prev(capacity_number_of_words + 1, 0);
    std::vector<uint64_t> values_next(capacity_number_of_words + 1, 0);
    values_next[0] = 1;

    Weight weight_sum = 0;
    std::vector<Weight> number_of_words_to_shift;
    std::vector<Weight> number_of_bits_to_shift_left;
    std::vector<Weight> number_of_bits_to_shift_right;
    std::vector<Weight> word_write;
    for (GroupId group_id = 0; group_id < instance.number_of_groups(); ++group_id) {
        // Check time
        if (parameters.timer.needs_to_end()) {
            algorithm_formatter.end();
            return output;
        }

        Weight weight_max = 0;
        for (ItemId item_id: instance.group(group_id).item_ids)
            weight_max = std::max(weight_max, instance.item(item_id).weight / gcd);
        weight_sum += weight_max;

        values_prev = values_next;

        for (ItemId item_id: instance.group(group_id).item_ids) {
            Weight item_weight = instance.item(item_id).weight / gcd;
            if (item_weight > capacity)
                continue;
            Weight number_of_words_to_shift = (item_weight >> 6);
            Weight number_of_bits_to_shift_left = item_weight - (number_of_words_to_shift << 6);
            Weight number_of_bits_to_shift_right = 64 - number_of_bits_to_shift_left;
            weight_sum += item_weight;
            Weight word_read = std::min(capacity_number_of_words, (weight_sum >> 6));
            Weight word_write = word_read - number_of_words_to_shift;
            if (number_of_bits_to_shift_left != 0) {
                while (word_write > 0) {
                    values_next[word_read] |= (values_prev[word_write] << number_of_bits_to_shift_left);
                    values_next[word_read] |= (values_prev[word_write - 1] >> number_of_bits_to_shift_right);
                    word_read--;
                    word_write--;
                }
                values_next[word_read] |= (values_prev[word_write] << number_of_bits_to_shift_left);
            } else {
                while (word_write >= 0) {
                    values_next[word_read] |= values_prev[word_write];
                    word_read--;
                    word_write--;
                }
            }
        }

        //for (Weight word = 0; word < number_of_words; ++word)
        //    for (int bit = 0; bit < 64; ++bit)
        //        std::cout << ((values[word] >> bit) & 1);
        //std::cout << std::endl;

        if (((values_next[capacity_number_of_words] >> capacity_number_of_bits_to_shift) & 1) == 1)
            break;
    }

    Weight opt = -1;
    for (Weight word = capacity_number_of_words; opt == -1; --word) {
        //std::cout << "word " << word << " / " << capacity_number_of_words << std::endl;
        if (values_next[word] == 0)
            continue;
        for (int bit = 63; bit >= 0; --bit) {
            Weight w = 64 * word + bit;
            //std::cout << "w " << w << " val " << ((values_next[word] >> bit) & 1) << std::endl;
            if (w <= capacity
                    && ((values_next[word] >> bit) & 1) == 1) {
                opt = gcd * w;
                break;
            }
        }
    }

    // Update value.
    algorithm_formatter.update_value(opt, "algorithm end (value)");
    // Update bound.
    algorithm_formatter.update_bound(opt, "algorithm end (bound)");

    algorithm_formatter.end();
    return output;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////// dynamic_programming_bellman_word_ram_rec ///////////////////
////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> dynamic_programming_bellman_word_ram_rec_opts(
        const Instance& instance,
        Weight gcd,
        GroupId group_id_1,
        GroupId group_id_2,
        Weight capacity,
        const Parameters& parameters)
{
    Weight capacity_number_of_words = (capacity >> 6);
    Weight capacity_number_of_bits_to_shift = capacity - (capacity_number_of_words << 6);
    std::vector<uint64_t> values_prev(capacity_number_of_words + 1, 0);
    std::vector<uint64_t> values_next(capacity_number_of_words + 1, 0);
    values_next[0] = 1;

    Weight weight_sum = 0;
    for (GroupId group_id = group_id_1; group_id < group_id_2; ++group_id) {

        // Check time
        if (parameters.timer.needs_to_end())
            return {};

        Weight weight_max = 0;
        for (ItemId item_id: instance.group(group_id).item_ids)
            weight_max = std::max(weight_max, instance.item(item_id).weight / gcd);
        weight_sum += weight_max;

        values_prev = values_next;

        for (ItemId item_id: instance.group(group_id).item_ids) {
            Weight item_weight = instance.item(item_id).weight / gcd;
            if (item_weight > capacity)
                continue;
            Weight number_of_words_to_shift = (item_weight >> 6);
            Weight number_of_bits_to_shift_left = item_weight - (number_of_words_to_shift << 6);
            Weight number_of_bits_to_shift_right = 64 - number_of_bits_to_shift_left;
            weight_sum += item_weight;
            Weight word_read = std::min(capacity_number_of_words, (weight_sum >> 6));
            Weight word_write = word_read - number_of_words_to_shift;
            if (number_of_bits_to_shift_left != 0) {
                while (word_write > 0) {
                    values_next[word_read] |= (values_prev[word_write] << number_of_bits_to_shift_left);
                    values_next[word_read] |= (values_prev[word_write - 1] >> number_of_bits_to_shift_right);
                    word_read--;
                    word_write--;
                }
                values_next[word_read] |= (values_prev[word_write] << number_of_bits_to_shift_left);
            } else {
                while (word_write >= 0) {
                    values_next[word_read] |= values_prev[word_write];
                    word_read--;
                    word_write--;
                }
            }
        }
        for (Weight c = 0; c <= capacity; ++c) {
            Weight word = (capacity >> 6);
            Weight bits = capacity - (word << 6);
        }
    }

    return values_next;
}

void dynamic_programming_bellman_word_ram_rec_rec(
        const Instance& instance,
        Weight gcd,
        GroupId group_id_1,
        GroupId group_id_2,
        Weight capacity,
        Solution& solution,
        const Parameters& parameters)
{
    ItemId k = (group_id_1 + group_id_2 - 1) / 2 + 1;

    auto values_1 = dynamic_programming_bellman_word_ram_rec_opts(
            instance,
            gcd,
            group_id_1,
            k,
            capacity,
            parameters);
    if (parameters.timer.needs_to_end())
        return;
    auto values_2 = dynamic_programming_bellman_word_ram_rec_opts(
            instance,
            gcd,
            k,
            group_id_2,
            capacity,
            parameters);
    if (parameters.timer.needs_to_end())
        return;

    Weight capacity_1_best = 0;
    Weight capacity_2_best = 0;
    Weight capacity_1 = 0;
    Weight capacity_2 = capacity + 1;
    for (;;) {
        if (capacity_1 + capacity_2 > capacity) {
            // Decrease capacity_2.
            for (;;) {
                capacity_2--;
                Weight word_2 = (capacity_2 >> 6);
                Weight bits_2 = capacity_2 - (word_2 << 6);
                if (capacity_2 < 0 || ((values_2[word_2] >> bits_2) & 1) == 1)
                    break;
            }
        } else {
            // Increase capacity_1.
            for (;;) {
                capacity_1++;
                Weight word_1 = (capacity_1 >> 6);
                Weight bits_1 = capacity_1 - (word_1 << 6);
                if (capacity_1 > capacity || ((values_1[word_1] >> bits_1) & 1) == 1)
                    break;
            }
        }
        if (capacity_2 < 0 || capacity_1 > capacity)
            break;
        //std::cout << "capacity_1 " << capacity_1 << " capacity_2 " << capacity_2 << std::endl;
        if (capacity_1 + capacity_2 <= capacity
                && capacity_1 + capacity_2 > capacity_1_best + capacity_2_best) {
            capacity_1_best = capacity_1;
            capacity_2_best = capacity_2;
        }
    }
    //std::cout
    //    << "n1 " << group_id_1
    //    << " k " << k
    //    << " n2 " << group_id_2
    //    << " c " << capacity
    //    << " capacity_1_best " << capacity_1_best
    //    << " capacity_2_best " << capacity_2_best
    //    << " capacity_1_best + capacity_2_best " << capacity_1_best + capacity_2_best
    //    << std::endl;

    if (group_id_1 == k - 1) {
        for (ItemId item_id: instance.group(group_id_1).item_ids) {
            if (capacity_1_best == instance.item(item_id).weight / gcd) {
                solution.add(item_id);
                break;
            }
        }
    }
    if (k == group_id_2 - 1) {
        for (ItemId item_id: instance.group(k).item_ids) {
            if (capacity_2_best == instance.item(item_id).weight / gcd) {
                solution.add(item_id);
                break;
            }
        }
    }

    if (capacity_1_best > 0 && group_id_1 != k - 1) {
        dynamic_programming_bellman_word_ram_rec_rec(
                instance,
                gcd,
                group_id_1,
                k,
                capacity_1_best,
                solution,
                parameters);
    }
    if (capacity_2_best > 0 && k != group_id_2 - 1) {
        dynamic_programming_bellman_word_ram_rec_rec(
                instance,
                gcd,
                k,
                group_id_2,
                capacity_2_best,
                solution,
                parameters);
    }
}

Output multiplechoicesubsetsumsolver::dynamic_programming_bellman_word_ram_rec(
        const Instance& instance,
        const Parameters& parameters)
{
    Output output(instance);
    AlgorithmFormatter algorithm_formatter(parameters, output);
    algorithm_formatter.start("Dynamic programming - Bellman - word RAM - recursve scheme");
    algorithm_formatter.print_header();

    Solution solution(instance);
    // Without group, the first half of the recursion would read group '0'.
    if (instance.number_of_groups() > 0) {
        Weight gcd = compute_weights_gcd(instance);
        dynamic_programming_bellman_word_ram_rec_rec(
            instance,
            gcd,
            0,
            instance.number_of_groups(),
            instance.capacity() / gcd,
            solution,
            parameters);
    }
    if (parameters.timer.needs_to_end()) {
        algorithm_formatter.end();
        return output;
    }

    // Update solution.
    algorithm_formatter.update_solution(solution, "algorithm end (solution)");
    // Update bound.
    algorithm_formatter.update_bound(solution.weight(), "algorithm end (bound)");

    algorithm_formatter.end();
    return output;
}
