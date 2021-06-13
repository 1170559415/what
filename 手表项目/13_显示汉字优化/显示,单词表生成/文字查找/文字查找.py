with open("结果.txt","w") as file:
    file.write("")
f1 = open("output12x12.chinese")
f2 = open("要查找的字.txt")
f3 = open("结果.txt","a")
r1 = f1.readline()
r2 = f2.readline()
r1_list = []
while r1 :
    r1_list.append(r1[0:len(r1)-1])
    r1 = f1.readline()
while r2 :
    for i in range(len(r2)) :
        for o in range(len(r1_list)) :
            if r2[i:i+1] in r1_list[o] :
                f3.write(r1_list[o][r1_list[o].find(':') + 1 : len(r1_list[o])])
                f3.write("$")
                break
    r2 = f2.readline()
    f3.write("\n")
f1.close
f2.close
f3.close
