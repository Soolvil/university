import sys

data = open(sys.argv[1], "r")
text = data.readline()
res = str()
for word in text.split():
    if len(word) > 2:
        res += word[0]
print(res)