valid = False

x = 0
while valid == False:
    x = input("insert input here: ")
    if (x.isnumeric()==False):
        continue
    if (isinstance(int(x), int) == False) or (int(x) < 1) or (int(x) > 8):
            continue

    else:
        valid = True

x = int(x)

output = ""
for i in range(1,x+1):
    output += " " * (x-i)
    output += "#" * i
    output += " "
    output = output + output[::-1]
    #output += "\n"
    print(output.rstrip())
    output = ""