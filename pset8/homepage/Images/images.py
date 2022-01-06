import os

filenames = next(os.walk("D:\Projects\Programming\cs50\pset8\homepage\Images"), (None, None, []))[2]  # [] if no file
#print(filenames)
newlist = []
extensions = []
for k in filenames:
    newlist.append(os.path.splitext(k)[0])
    extensions.append(os.path.splitext(k)[1])

for i in range(len(newlist)):
    print("\'" + newlist[i].split()[0] + "\'")


with open("D:\Projects\Programming\cs50\pset8\homepage\Images\output.txt" , "w") as f:
    f.truncate(0)
    for i in range(len(filenames)):
        if extensions[i] == ".jpg" or extensions[i] == ".jpeg" or extensions[i] == ".png":
            f.write('<div class="brick">\n    <img src="Images/'+ filenames[i] + '" class="'+newlist[i].split()[0]+'" alt="'+newlist[i]+'" title="'+newlist[i]+'">\n</div>\n')










