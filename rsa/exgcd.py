#! /usr/bin/env python3
import sys;
sys.path.append("../")
import task_2
import os
import signal

def handler():
    pass

SIZE = 128
r = ord(sys.argv[1][0])
w = ord(sys.argv[1][1])
r = os.fdopen(r, "rb")
w = os.fdopen(w, "wb")
signal.signal(signal.SIGUSR2, handler)
#signal.pause()
a = b''
b = b''
readlen = 0
while(readlen < SIZE):
    a = a+r.read(SIZE)
    readlen = len(a)

readlen = 0
while(readlen < SIZE):
    b = b+r.read(SIZE)
    readlen = len(b)

numa = 0
numb = 0
a = a.decode("utf-8")
b = b.decode('utf-8')
#a = bytes(a, "utf-8")
#print(type(a))
# i = 0
# while(i<SIZE):
#     numa = numa<<8+ord(a[i])
#     i=i+1
for i in a:
    numa = (numa<<8)+ord(i)

# i = 0
# while(i<SIZE):
#     numb = numb<<8+ord(b[i])
#     i=i+1
for i in b:
    numb = (numb<<8)+ord(i)

for i in a:
    print(ord(i))
print(numa);
print(numb)
numresult = task_2.rem(numa, numb)

result = b''
i = 0
while(i<SIZE):
    result = result+bytes(chr((numresult>>(SIZE-i-1)*8)%256), 'utf-8')
    i=i+1
w.write(result)

