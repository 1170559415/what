import struct
with open("Chinese.Ch","w") as file:
    file.write("")
f3 = open("Chinese.Ch" , "ab+")
Chinese = ":255^255"
font = struct.pack('B',int(Chinese[Chinese.find(':')+1:Chinese.find('^')]))
number = struct.pack('B',int(Chinese[Chinese.find('^')+1:len(Chinese)]))
f3.write(font)
f3.write(number)
f3.close
