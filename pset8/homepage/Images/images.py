import os

filenames = next(os.walk(os.getcwd()), (None, None, []))[2]  # [] if no file
#print(filenames)
newlist = []
extensions = []
for k in filenames:
    newlist.append(os.path.splitext(k)[0])
    extensions.append(os.path.splitext(k)[1])

for i in range(len(newlist)):
    print("\'" + newlist[i].split()[0] + "\'")


with open("output.txt" , "w") as f:
    f.truncate(0)
    for i in range(len(filenames)):
        if extensions[i] == ".jpg" or extensions[i] == ".jpeg" or extensions[i] == ".png":
            filename = filenames[i]
            classname = newlist[i].split()[0]
            altname = newlist[i]
            f.write('<div class="grid-item" data-src="Images/'+filename+'">\n    <img src="Images/'+ filename + '" class="'+classname+'" alt="'+altname+'" title="'+altname+'">\n</div>\n')










