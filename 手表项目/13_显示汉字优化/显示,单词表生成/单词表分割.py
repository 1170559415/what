f1 = open("english8000.txt")
number = 0
name = 0
r1 = f1.readline()
string = r1[1:2]
while r1 :
    if string != r1[1:2] :
        string = r1[1:2]
        number = 0
        name = 0
    f2 = open(r1[1:2] + str(name) + ".elist","a")
    if number == 0:
        f2.write(r1[1:len(r1)-1])
    else :
        f2.write(r1[0:len(r1)-1])
    number = number + 1
    if number == 90 :
        number = 0
        name = name + 1
        f2.write('$')
    r1 = f1.readline()
    f2.close
f1.close
