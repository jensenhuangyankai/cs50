
legit = False
x = input("input a credit card number here: ")

if (x.isnumeric() == False):
    print("INVALID")


reverseX = x[::-1]



multipliedList = []
nonMultipliedList = []
for i in range(0,len(reverseX)):
    if i % 2 == 1:
        #print(int(reverseX[i]))
        multipliedList.append(int(reverseX[i]))
    else:
        nonMultipliedList.append(int(reverseX[i]))

multiplied = 0
notMultiplied = 0

for i in range(len(multipliedList)):
    number = multipliedList[i] * 2

    for j in range(len(str(number))):
        multiplied += int(str(number)[j])

#print(multiplied)

for i in range(len(nonMultipliedList)):
    notMultiplied += nonMultipliedList[i]

#print(notMultiplied)

total = multiplied + notMultiplied
#print(total)
#print(legit)
if int(str(total)[-1]) == 0:
    legit = True

#card number validation works


if legit:
    if int(x[0]) == 3: #amex
        if int(x[1]) == 4 or int(x[1]) == 7:
            print("AMEX")
    if int(x[0]) == 5:
        if int(x[1]) in [1,2,3,4,5]:
            print("MASTERCARD")
    if int(x[0]) == 4:
        print("VISA")
else:
    print("INVALID")