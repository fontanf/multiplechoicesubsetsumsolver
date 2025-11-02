import argparse
import sys
import os
import json
import shutil


def run_command(command):
    print(command)
    status = os.system(command)
    if status != 0:
        sys.exit(1)
    print()


parser = argparse.ArgumentParser(description='')
parser.add_argument(
        "algorithm",
        type=str,
        help='')

args = parser.parse_args()


main = os.path.join(
        "install",
        "bin",
        "multiplechoicesubsetsumsolver")

output_path = os.path.join(
        "data",
        f"test_{args.algorithm.replace("-", "_")}.txt")
with open(output_path, 'w') as output_file:
    for number_of_groups, number_of_items_in_groups in [
            (10, 10), (10, 100), (100, 10), (100, 100)]:
        for data_range in [1000]:
            for capacity_ratio in range(101):
                p = (
                        f"g{number_of_groups}"
                        f"_ni{number_of_items_in_groups}"
                        f"_r{data_range}")
                instance_path = os.path.join(
                        "tests",
                        p,
                        f"instance_{p}_c{capacity_ratio}")
                instance_full_path = os.path.join("data", instance_path)
                certificate_full_path = f"{instance_full_path}_solution.txt"
                certificate_tmp_path = f"solution.txt"
                json_output_path = "output.json"
                command = (
                        main
                        + f"  --verbosity-level 1"
                        + f"  --input \"{instance_full_path}\""
                        + f"  --algorithm {args.algorithm}"
                        + f"  --certificate \"{certificate_tmp_path}\""
                        + f"  --output \"{json_output_path}\""
                        + f"  --time-limit 1")
                run_command(command)

                # If solved to optimality, add to output file.
                json_output_file = open(json_output_path, "r")
                json_data = json.load(json_output_file)
                bound = int(json_data["Output"]["Bound"])
                value = int(json_data["Output"]["Value"])
                if bound == value:
                    output_file.write(f"{instance_path}\n")
                    shutil.copyfile(
                            certificate_tmp_path,
                            certificate_full_path)
