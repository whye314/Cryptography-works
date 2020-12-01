from task_2 import rem
import random


KEY_LEN = 2**10
MAX = 2**(KEY_LEN)
PQ = 2**(KEY_LEN/2)

def gcd(a, m):
    while(1):
        if m%a == 0:
            return a
        t = a
        a = m%a
        m = t


def pow_mod(a, b, c):
    a = a%c
    r = 1
    # if b%2 == 1:
    #     return (a*pow_mod(a, b-1, c))%c
    # if b%2 == 0:
    #     return pow_mod((a*a)%c, b/2, c)
    while(b>0):
        k = b%2
        b = b//2
        if k == 1:
            r = (r*a)%c
        a = (a*a)%c
    return r




class alice:
    def __init__(self):
        self.p, self.q, self.n, self.e, self.d = create_key()
    
    def en(self, m):
        return pow_mod(m, self.e, self.n)
    
    def de(self, m):
        return pow_mod(m, self.d, self.n)


def rabin(n):
    if n == 2:
        return True
    if n&1 == 0 or n<2:
        return False
    m,s = n - 1,0
    while m&1==0:
        m = m>>1
        s += 1
    for _ in range(3):
        b = pow_mod(random.randint(2,n-1), m, n)
        if b==1 or b== n-1:
            continue
        for __ in range(s-1):
            b = pow_mod(b, 2, n)
            if b == n-1:
                break
        else:
            return False
    return True


def get_prime(len):
    while(1):
        r = random.randint(0, len)
        if(rabin(r)):
            return r


def create_key():
    p = get_prime(PQ)
    q = get_prime(PQ)
    n = q*p
    psi = (p-1)*(q-1)
    while(1):
        e = random.randint(2, psi)
        if gcd(e, psi)==1:
            break
    d = rem(e, psi)
    if(d<0):
        d = d + psi

    return [p, q, n, e, d]


if __name__ == '__main__':
    a = alice()
    m = 123452346524564256245908312456308497560238475620438756924837569287436592837456918322438759623847569137845691387465
    print(a.en(m))
    print()
    print(a.p)
    print()
    print(a.q)
    print()
    print(a.e)
    print()
    print(a.d)
    print(a.de(a.en(m)))

