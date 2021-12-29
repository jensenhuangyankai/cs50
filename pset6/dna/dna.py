import sys
import csv


def repeat(sequence,dna): #finds how many times the sequence repeats
    longest = 0

    index = sequence.find(dna)
    while index < len(sequence) and index != -1:
        counter = 0
        while sequence[index:index+len(dna)] == dna:
            counter+=1
            #print(sequence[index:index+len(dna)])
            #print(counter)
            if counter > longest:
                longest = counter
            index += len(dna)
        index = sequence.find(dna,index+1)

    return longest


if len(sys.argv) != 3:
    sys.exit("Usage: python dna.py data.csv sequence.txt")

else:
    data_csv = open(sys.argv[1])
    reader = csv.DictReader(data_csv)
    strs = []

    headers = reader.fieldnames
    for key in headers:
        if key == "name":
            pass
        else:
            strs.append(key)

    #imports the STRs correctly

    data_file = open(sys.argv[2]) #sequence.txt, actual data
    data = data_file.read()    #reads the file correctly

    results = {}
    for item in strs:
        results[item] = repeat(data,item)
        #print("strs: "+item+ "longest value is:" + str(repeat(data,item)))

    #print(results)

    #reader = csv.DictReader(data_csv)
    match = False
    for row in reader:
        #print("\n")
        #print(row)
        total = 0
        for item in strs:

            #print(item)

            #print("row is"+str(row))
            #print("wtf")

            #print("row:"+str(row[item]))
            #print("results:"+str(results[item]))
            #print(type(row[item]))
            #print(type(results[item]))
            if int(row[item]) == results[item]:
                #print("row:"+str(row[item]))
                #print("results:"+str(results[item]))
                total+=1
                #print(same)

        #print(same)
        #print(len(strs))
        if total==len(strs):

            print(row["name"])
            match = True
    if match == False:
        print("No match")





