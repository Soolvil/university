import re
import sys

data = open(sys.argv[1], "r")
text = str()
for line in data:
    text += line
sentences = re.split(r'(?<=[.?!]) +', text)
for sentence in sentences:
    print(sentence)
print(f"Предложений в тексте: {len(sentences)}")