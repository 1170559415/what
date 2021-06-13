f1 = open("input12x12.b")
f3 = open("output12x12.chinese","w")
r1 = f1.readline()
number = 0
name = 0
while r1 :
    f2 = open(str(name) + ".ch" ,"a")
    if(r1.find('/') == -1) :
        f2.write(r1[0:len(r1)-1] + "$")
        number = number + 1
        f2.close
    else :
        f3.write(r1[r1.find('(')+1:r1.find(')')] + " :" + str(name)+"^"+str(number) + "\n")
    if number == 25:
        number = 0
        name = name + 1
    r1 = f1.readline()
f1.close
f3.close
