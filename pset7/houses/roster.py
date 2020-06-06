import cs50
from sys import argv

# Check arguments
if len(argv) != 2:  # check arguments
    print("You should select the House")
    exit(1)

# Database
db = cs50.SQL("sqlite:///students.db")

# Request
students = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])

# Format data for output
result = ''
for student in students:
    if student['middle']:
        name = " ".join([student['first'], student['middle'], student['last']])
    else:
        name = " ".join([student['first'], student['last']])
    result += "{}, born {}\n".format(name, student['birth'])

print(result)
