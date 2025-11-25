import sys

counter = 1
separator = "--------------------------------------------------------------------------"

input_data = (open(sys.argv[1], "r")).readlines()
commands = (open(sys.argv[2], "r")).readlines()
output_file = open(sys.argv[3], "w")

def encode(text):
    res = ""
    buff = ""
    for i in range(1, len(text)):
        if(text[i] != text[i - 1]):
            if(len(buff) <= 2):
                res += buff
                buff = ""
                continue
            res += len(buff) + buff[0]
            buff = ""
        buff += text[i]
    return res

def decode(seq):
    text = ""
    i = 0
    while i < len(seq):
        if(seq[i] >= '3' and seq[i] <= '9'):
            text += seq[i + 1] * int(seq[i])
            i += 2
            continue
        text += seq[i]
        i += 1
    return text

def search_seq(data, key):
    for protein in data:
        _, _, amino = protein
        if amino.find(key) != -1:
            return protein
    return ("", "", "")

def search_name(data, key):
    for protein in data:
        name, _, _ = protein
        if name == key:
            return protein
    return ("", "", "")

def diff(amino1, amino2):
    res = abs(len(amino1) - len(amino2))
    for i in range(0, min(len(amino1), len(amino2))):
        if amino1[i] != amino2[i]:
            res += 1
    return res

def mode(seq):
    acids = {f"{seq[0]}":0}
    for i in range(0, len(seq)):
        if seq[i] not in acids:
            acids[seq[i]] = 0
        acids[seq[i]] += 1
    maximal = seq[0]
    for name, count in acids.items():
        if count > acids[maximal] or (count == acids[maximal] and name < maximal):
            maximal = name
    return (maximal, acids[maximal])

print("Pupkevich Dzmitry\nGenetic Searching", file = output_file)
data = []
for i in range(0, len(input_data)):
    temp = input_data[i].split('\t')
    temp[-1] = temp[-1][:-1]
    data.append((temp[0], temp[1], temp[2]))
#print(data)
#print("\n\n\n\n")
for line in commands:
    print(separator, file = output_file)
    arguments = line.split('\t')
    arguments[-1] = arguments[-1][:-1]
    if arguments[0] == "search":
        print(f"{counter:0>3}   search   {decode(arguments[1])}", file = output_file)
        print("organism                 protein", file = output_file)
        res = search_seq(data, decode(arguments[1]))
        name, organism, _ = res
        if name != "":
            print(f"{organism}     {name}", file = output_file)
        else:
            print("NOT FOUND", file = output_file)
    elif arguments[0] == "diff":
        print(f"{counter:0>3}   diff   {arguments[1]}   {arguments[2]}", file = output_file)
        first = search_name(data, arguments[1])[2]
        second = search_name(data, arguments[2])[2]
        if first != "" and second != "":
            res = diff(first, second)
            print("amino-acids difference:", file = output_file)
            print(f"{res}", file = output_file)
        else:
            print("MISSING:", file = output_file)
            if first == "":
                print(arguments[1])
            if second == "":
                print(arguments[2])
    elif arguments[0] == "mode":
        print(f"{counter:0>3}   mode   {arguments[1]}", file = output_file)
        acid_seq = search_name(data, arguments[1])[2]
        if acid_seq != "":
            amino, count = mode(acid_seq)
            print("amino-acid occurs:", file = output_file)
            print(f"{amino}          {count}", file = output_file)
        else:
            print("MISSING:", file = output_file)
            print({arguments[1]}, file = output_file)
    counter += 1
print(separator, file = output_file)