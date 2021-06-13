f1 = open("单词汇表.txt")
f2 = open("单词汇表2.txt")
f3 = open("请重命名.list","a")
r1 = f1.readline()
r2 = f2.readline()
number = 0;
while r1 :
    if r1.__eq__(r2) == 0 :
        r3 = "$" + r1[0:len(r1)-1] + "/"+str(number)+ ".c" + "\n"
        number = number + 1;
        f3.write(r3)
    r1 = f1.readline()
    r2 = f2.readline()
f1.close
f2.close
f3.close
