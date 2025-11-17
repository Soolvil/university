import sys

def rem_brackets(text):
    text = str(text)
    while text.find('(') != -1:
        lbracket = text.rfind('(')
        rbracket = (text[lbracket:]).find(')') + lbracket
        text = text[:lbracket] + text[rbracket + 1:]
    return text

data = open(sys.argv[1], "r")
text = str()
for line in data.readlines():
    text += line
text = rem_brackets(text)
print(text)