import csv
import cs50
from sys import argv

# Check arguments
if len(argv) != 2:  # check arguments
    print("You should point the CSV")
    exit(1)

# Database
db = cs50.SQL("sqlite:///students.db")

with open(argv[1], 'r') as file:
    reader = csv.DictReader(file)

    # select data from csv for fields
    for row in reader:
        full_name = row['name'].split(' ')  # split name on parts
        first_name = full_name[0]
        last_name = full_name[-1]
        if len(full_name) > 2:
            middle_name = full_name[1]
        else:
            middle_name = None
        house = row["house"]
        birth = row['birth']

        # insert data into table
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                   first_name, middle_name, last_name, house, birth)
exit(0)