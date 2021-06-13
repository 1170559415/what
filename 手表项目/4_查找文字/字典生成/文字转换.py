with open("output12x12.c","w") as file:
    file.write("")
f1 = open("11.c")
f2 = open("output12x12.c",'a')
s = f1.readline()
while s:
    s = f1.readline()
    f2.write(s)
    s = f1.readline()
    for i in range(24):
        if i == 0:
            a = i*4
        else:
            a = i*5
        b = a + 4
        w = eval(s[a:b])
        w = str(w)
        f2.write(w)
        if i == 23:
            f2.write(",\n")
        else:
            f2.write(",")
f1.close
f2.close
