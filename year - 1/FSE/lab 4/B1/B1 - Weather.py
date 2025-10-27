import sys
import math

file = open(sys.argv[1], "r")
months = {
    "January" : int(31),
    "February" : int(28),
    "March" : int(31),
    "April" : int(30),
    "May" : int(31),
    "June" : int(30),
    "July" : int(31),
    "August" : int(31),
    "September" : int(30),
    "October" : int(31),
    "November" : int(30),
    "December" : int(31),
}

def valid_day(month, cur_day):
    if(cur_day < 0):
        return False
    if(cur_day > months[month]):
        return False
    return True

print("Programmer: Pupkevich Dzmitry")
project = file.readline()
project = project[:2] + " " + project[2:-1]
place = file.readline()
place = place[:-1]
cur_month, year = file.readline().split(',')
year = int(year)
if(year % 400 == 0 or (year % 4 == 0 and year % 100 != 0)):
    months["December"] = 29
errors = "Error         Day       Line\n"
days = [float(-1)] * months[cur_month]
iter = 3
day_count = 0
minimal = float()
maximal = float()
average = float()
for line in file.readlines():
    iter = iter + 1
    cur_day, value = line.split()
    cur_day = int(cur_day)
    value = float(value)
    if(not valid_day(cur_month, cur_day)):
        errors += f"Invalid       {cur_day:>3}       {iter:>4}\n"
        continue
    if(days[cur_day - 1] != -1):
        errors += f"Repeated      {cur_day:>3}       {iter:>4}\n"
        continue
    days[cur_day - 1] = value
    maximal = max(maximal, value)
    minimal = min(minimal, value)
    average = average + value
    day_count = day_count + 1
average = average / len(days)
print(project)
print()
print(f"Precipitation report for {place} during {cur_month}, {year}")
print()
print(errors)
print("Day Amount Graph")
for i in range(0, len(days)):
    print(f"{i + 1:>3}   ", end = '')
    if(days[i] == -1):
        print("  NA")
        continue
    print(f"{days[i]:.2f} {"*" * int(math.ceil(float(days[i]) / 0.25))}")
print()
print("Minimum     Maximum     Average")
if(day_count == 0):
    print("     NA          NA          NA")
else:
    print(f"   {minimal:.2f}        {maximal:.2f}        {average:.2f}")