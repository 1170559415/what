f1 = open("output12x12.b")
r1 = f1.readline()
r1_list = []
while r1 :
    r1_list.append(r1[0:len(r1)-1])
    r1 = f1.readline()
f3 = open("词典.txt")
r3 = f3.readline()
number = 0;
while r3 :
    f2 = open(str(number) + ".c" ,"a")
    r3 = r3[r3.find('[')+1 : r3.find('\n')]
    for i in range(len(r3)) :
        for o in range(len(r1_list)) :
            if r3[i:i+1] in r1_list[o] :
                f2.write(r1_list[o + 1])
                f2.write("$")
                break
    number = number + 1;
    r3 = f3.readline()
f1.close
f3.close