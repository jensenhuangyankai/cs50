text = input("Text: ")

words,letters,sentences = 0,0,0

i = 0
while i  < len(text):
    #print(i)
    #print(text[i],end ="")
    
    if text[i] == '\'':
        i+=1
    
    if text[i] == ',':
        letters -= 1
        
    
    if (text[i] == '.' or text[i] == '?' or text[i] == '!'):
        sentences += 1
        words += 1
        i+=1
        #print("word")

    elif (text[i] == ' '):
        words += 1
        #print("word")

    else:
        letters += 1
    i += 1
        


L = letters * (100.0/words)
S = sentences * (100.0/words)
index = 0.0588 * L - 0.296 * S - 15.8
print()
print(str(letters))
print(str(sentences)) 
print(str(words))

if (index < 1):
    print("Before Grade 1")
elif (index > 16):
    print("Grade 16+")
else:
    print("Grade " + str(round(index)))