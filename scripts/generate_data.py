import os

generator_main = os.path.join(
        "install",
        "bin",
        "multiplechoicesubsetsumsolver_generator")

for number_of_groups, number_of_items_in_groups in [
        (10, 10), (10, 100), (10, 1000),
        (100, 10), (100, 100), (100, 1000),
        (1000, 10), (1000, 100), (10000, 10)]:
    for data_range in [1000, 10000]:
        for capacity_ratio in range(101):
            p = (
                    f"g{number_of_groups}"
                    f"_ni{number_of_items_in_groups}"
                    f"_r{data_range}")
            output_path = os.path.join(
                    "data",
                    "tests",
                    p,
                    f"instance_{p}_c{capacity_ratio}")
            if not os.path.exists(os.path.dirname(output_path)):
                os.makedirs(os.path.dirname(output_path))
            command = (
                    generator_main
                    + f"  --number-of-groups \"{number_of_groups}\""
                    + f"  --number-of-items-in-groups \"{number_of_items_in_groups}\""
                    + f"  --data-range \"{data_range}\""
                    + f"  --capacity-ratio \"{capacity_ratio / 100}\""
                    + f"  --seed 0"
                    + f"  --output \"{output_path}\"")
            print(command)
            status = os.system(command)
