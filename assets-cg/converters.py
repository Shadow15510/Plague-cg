import fxconv


def convert(input_name, output, params, target):
    if params["custom-type"] == "mutation-table":
        convert_mt(input_name, output, params, target)
        return 0
    elif params["custom-type"] == "mutation":
        convert_md(input_name, output, params, target)
        return 0
    else:
        return 1


def convert_mt(input_name, output, params, target):
    with open(input_name, "r") as file:
        # Extract informations
        lines = file.read().splitlines()
        mutation_matrix = [i.split(" | ") for i in lines]
        
        # Encode information into bytes
        data = bytes()
        for i in mutation_matrix:
            for j in i:
                data += fxconv.u32(int(j))

    fxconv.elf(data, output, "_" + params["name"], **target)

