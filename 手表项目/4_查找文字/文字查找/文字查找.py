with open("结果.txt","w") as file:
    file.write("")
f1 = open("output12x12.c")
f2 = open("要查找的字.txt")
f3 = open("结果.txt","a")
r1 = f1.readline()
r2 = f2.readline()
r_list = []
while r1 :
    t = 1
    for i in range(len(r2)) :
        if r2[i:i+1] in r1 :
            r_list.append(r1)
            r1 = f1.readline()
            r_list.append(r1)
            r1 = f1.readline()
            t = 0
    if t :
        r1 = f1.readline()
        r1 = f1.readline()
for o in range(len(r2)):
    for i in range(len(r_list)) :
        if r2[o:o+1] in r_list[i] :
            f3.write(r_list[i])
            f3.write(r_list[i+1])
f1.close
f2.close
f3.close
