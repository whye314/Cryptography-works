#!/usr/bin/python
# -*- coding: UTF-8 -*-
#whye

import time

class stack:
    def __init__(self):
        self.array = []
    
    def push(self, n):
        # st = stack_node()
        # st.index = len(self.array)
        # st.node = n
        self.array.append(n)
    def pop(self):
        return self.array.pop()
    def get(self, n):
        return self.array[n]
    def get_len(self):
        return len(self.array)

class stack_node:
    def __init__(self):
        self.index = 0
        self.node = 0

class node:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def add(self, a, b):
        tn = node(0, 0)
        tn.x = a.x + b.x
        tn.y = a.y + b.y
        return tn
    
    def mul(self, a):
        tn = node(self.x, self.y)
        tn.x *= a
        tn.y *= a
        return tn

def crt(a, m):
    if not len(a) == len(m):
        print('Error: There are different lengths of a and b.')
        return 0
    m_s = 1
    a_s = 0
    for m_i in m:
        m_s *= m_i
    for i in range(len(a)):
        # m_i = int(m_s/m[i])#大数除法有精度问题不能这样算，小的数可以这样算，这里找了好久 shabi
        m_i = m_s//m[i]
        rm = rem(m_i, m[i])
        c_i = m_i*rm
        a_s += a[i]*c_i
    return a_s%m_s


#Euclid算法计算逆元，算法见《信息安全数学基础》20页例1。
def rem(a, m):
    return rem1(a, m)

def rem1(a, m):
    min = _min(a, m)
    max = a+m-min
    array = stack()
    tn1 = node(1, max)
    tn2 = node(1, min)
    array.push(tn1)
    array.push(tn2)
    gcd = 0
    while(1):
        if max%min == 0:
            gcd = min
            break
        tn = node(max//min, max%min)
        array.push(tn)
        max = min
        min = tn.y
    rm = rem2(gcd, array)
    if a > m:
        return rm.x
    else:
        return rm.y
    return rm

def rem2(gcd, array):
    l = array.get_len()-1
    rm = node(0,0)
    rem3(rm, gcd, array, l)
    return rm

# def rem3(rm, gcd, array, n, x = 1):
#     if(n <= 1):
#         if(n == 0):
#             rm.x += x
#         else:
#             rm.y += x
#         return rm
#     rem3(rm, gcd, array, n-2, x)
#     rem3(rm, gcd, array, n-1, -(array.get(n).x)*x)

#上面的递归算法效率太低，第三题用不了，下面是优化后的算法

def rem3(r, gcd, array, n, x = 1):
    rm = []
    for i in range(array.get_len()):
        if i < 2:
            if i == 0:
                rm.append(node(1, 0))
            elif i == 1:
                rm.append(node(0, 1))
        else:
            tn = node(0, 0)
            tn = tn.add(rm[i-2], (rm[i-1].mul(-array.get(i).x)))
            rm.append(tn)
    r.x = rm[n].x
    r.y = rm[n].y

def _min(a, b):
    if a>b:
        return b
    return a

if __name__ == '__main__':
    start = time.perf_counter()
    m = [3,5,7]
    a = [2,4,6]
    a = [4164672228, 147282878, 802590088]
    m = [4439434147, 3563238423, 2602344563]
    print(crt(a, m))
    end = time.perf_counter()
    print('time cost: '+str(end - start))