import csv
from sys import argv


def load_dict_dna(file):
    """load dna of people in dict"""
    dict_dna = {}
    with open(file, encoding='utf-8') as dna_csv:
        reader = csv.DictReader(dna_csv)
        for row in reader:
            key = row.pop('name')
            dict_dna[key] = dict(row)
    return dict_dna


def load_dna(file):
    """load dna from txt file"""
    with open(file, 'r') as dna_file:
        dna = dna_file.readline()
    return dna


def is_target(dict_str, dna):
    """if target return true, else false"""
    str_value = {}
    for key in dict_str:
        value = 1
        while True:  # count the longest str for this key
            long_str = key * value
            if long_str in dna:
                value += 1
            else:
                break
        str_value[key] = str(value - 1)
    if str_value == dict_str:
        return True
    return False


def check_dna():
    """check lis of dna"""
    dict_dna = load_dict_dna(argv[1])
    dna = load_dna(argv[2])
    target_name = "No match"
    for name in dict_dna:  # check everybody
        if is_target(dict_dna[name], dna):
            target_name = name  # catch him!
    return target_name


if len(argv) != 3:  # check arguments
    print("You should point the CSV and the TXT file with DNA!")
else:
    print(check_dna())

