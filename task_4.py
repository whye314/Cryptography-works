#!/usr/bin/python
# -*- coding: UTF-8 -*-
#whye


import random
from task_1 import check
from task_3 import gcd
# class Alice:
#     def __init__(self):
#         self.g = random.randint(pow(10, 5), pow(10, 10))
#         self.x = random.randint(pow(10, 1), pow(10, 5))
#         self.h = pow(self.g, self.x)
#         self.pk = [self.g, self.h]

#     def get_pk(self):
#         return self.pk

#     def decode(self, pm):
#         c_1 = pm[0]
#         c_2 = pm[1]
#         s = pow(c_1, self.x)
#         m = c_2//s
#         return m

#     def encode(self, m, pk):
#         g = pk[0]
#         h = pk[1]
#         y = random.randint(pow(10, 1), pow(10, 5))
#         c_1 = pow(g, y)
#         s = pow(h, y)
#         c_2 = m * s
#         pm = [c_1, c_2]
#         return pm

# if __name__ == '__main__':
#     a = Alice()
#     b = a.encode(1212412345135, a.get_pk())
#     c = a.decode(b)
#     print(c)

class Alice:
    def __init__(self):
        while(1):
            self.p = random.randint(pow(10, 100), pow(10, 200))
            if  check(self.p):
                break
        self.g = self.find_g(self.p)
        self.x = random.randint(1, self.p - 1)
        self.h = pow(self.g, self.x, self.p)
        self.pk = [self.p, self.g, self.h]

    def find_g(self, p):
       for i in range(2, p):
            if p == 2:
               return 1
            elif p == 3:
                return 2
            elif p == 4:
                return 3
            else:
                for j in self.p_prime(p-1):
                    if pow(i, (p-1)//j, p) == 1:
                        break
            return i

    def p_prime(self, p):
        p_prime = []
        for i in range(pow(p, 0.5) + 1):
            if p%i == 0:
                p_prime.append(i)
        return p_prime

    def encode(self, m, pk):
        p = pk[0]
        g = pk[1]
        h = pk[2]
        while(1):
            y = random.randint(1, p - 2)
            if gcd(y, p - 1) == 1:
                break
        s = pow(h, y, p)    
        c_1 = pow(g, y, p)
        c_2 = pow(m*s, 1, p)
        return  [c_1, c_2]


    def decode(self, c):
        c_1 = c[0]
        c_2 = c[1]
        s = pow(c_1, self.x, self.p)
        m = c_2//s
        return m

    def get_pk(self):
        return self.pk

if __name__ == "__main__":
    alice = Alice()
    m = alice.decode(alice.encode(132534534534534234634567345734567356734563456, alice.get_pk()))
    print(m)