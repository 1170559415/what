f1 = open("8000词.txt")
number = 0
name = 0
r1 = f1.readline()
while r1 :
    f2 = open("english8000.txt","a")
    f2.write("$"+r1[0 : r1.find('[')] + ":" + str(name) + "." + str(number) +"\n")
    number = number + 1
    if number == 30:
        number = 0
        name = name + 1
    r1 = f1.readline()
f1.close
f2.close
