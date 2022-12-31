import sys

lines = []
with open(sys.argv[1], "r") as file:
    for line in file:
        # Split the line with respect to the space character
        words = line.split(" ")
        if words[0] == "LD":
            operation = 0
            dst = int(words[1][1:])
            imm = int(words[2])
            src0 = 0
            src1 = 0
        elif words[0] == "ST":
            operation = 1
            dst = 0
            imm = int(words[2])
            src0 = 0
            src1 = int(words[1][1:])
        elif words[0] == "ADD":
            operation = 2
            dst = int(words[1][1:])
            imm = 0
            src0 = int(words[2][1:])
            src1 = int(words[3][1:])
        elif words[0] == "SUB":
            operation = 3
            dst = int(words[1][1:])
            imm = 0
            src0 = int(words[2][1:])
            src1 = int(words[3][1:])
        elif words[0] == "MULT":
            operation = 4
            dst = int(words[1][1:])
            imm = 0
            src0 = int(words[2][1:])
            src1 = int(words[3][1:])
        elif words[0] == "DIV":
            operation = 5
            dst = int(words[1][1:])
            imm = 0
            src0 = int(words[2][1:])
            src1 = int(words[3][1:])
        elif words[0] == "HALT":
            operation = 6
            dst = 0
            imm = 0
            src0 = 0
            src1 = 0
        output = (operation << 24) | (dst << 20) | (src0 << 16) | (src1 << 12) | imm
        lines.append("{0:0{1}x}".format(output, 8))

with open(sys.argv[2], 'w') as f:
  for line in lines:
    f.write(line + '\n')