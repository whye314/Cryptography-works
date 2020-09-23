#!/usr/bin/python
# -*- coding: UTF-8 -*-
#whye

import random

def check_once(a, m):
    if not gcd(a, m) == 1:
        return False
    return pow(a, m-1, m) == 1


def gcd(a, m):
    while(1):
        if m%a == 0:
            return a
        t = a
        a = m%a
        m = t


def check(m):
    for _times in range(1000):
        a = random.randint(0, m-2)
        if check_once(a, m) == False:
            return False
    return True
    

if __name__ == "__main__":
    m = 10000687781664254534533453324252244268578


    print('Within 1/2^1000 deviation, the outcome of '+ str(m) +' is ', end = '')
    print(check(m))