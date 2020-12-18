
import random
import hashlib


p=0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
a=0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
b=0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
n=0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
Gx=0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
Gy=0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
l = (len(hex(p))-1)//2



class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.inf = (x==0) and (y==0)#(0, 0) don't be included in y^2=x^3+ax+b, so (0, 0) means the infinity point

class Alice:
    def __init__(self, len):
        self.a = a
        self.b = b
        self.p = p
        self.n = n
        self.G = Point(Gx, Gy)
        self.k = random.randrange(1, self.n-1)
        self.K = self.mul(self.k, self.G)
        self.h = int(pow(pow(self.p, 0.5)+1, 2)//self.n)
        self.l = l

    
    def en(self, message, K):
        M = bytes(str(message), 'utf-8')
        r = random.randrange(1, self.n)
        S = self.mul(self.h, K)
        if(S.inf ):
            print('public key error')
            exit(1)
        C1 = self.mul(r, self.G)
        C1 = self.pointToByte(C1)
        P2 = self.mul(r, K)
        X2 = P2.x.to_bytes(self.l, 'big')
        Y2 = P2.y.to_bytes(self.l, 'big')
        t = self.KDF(X2+Y2, len(message))
        if int.from_bytes(t, 'big') == 0:
            print('KDF error')
            exit(1)
        #M = (self.l-len(M))*b'\x00' + M
        C2 = int.from_bytes(M, 'big') ^ int.from_bytes(t, 'big')
        C2 = C2.to_bytes(len(message), 'big')
        C3 = hashlib.md5(X2 + M + Y2).digest()
        return C1 + C2 + C3
        

    def de(self, C):
        PC = int(C[0])
        if PC == 00:
            print('error Ciphertext 1')
            exit(1)
        if PC == 2 or PC == 3:
            l = self.l
        else:
            l = 2*self.l
        C1 = C[0:1+l]
        C2 = C[1+l:len(C)-16]
        C3 = C[len(C) - 16:]
        C1 = self.byteTopoint(C1)
        if not pow(C1.y, 2, p)==(pow(C1.x, 3, p) + self.a*C1.x + self.b)%p:#(C1.y**2) == (C1.x**3 + self.a * C1.x + self.b):
            print('error Ciphertext 2')
            exit(1)
        S = self.mul(self.h, C1)
        if S.inf :
            print('error Ciphertext 3')
            exit(1)
        P2 = self.mul(self.k, C1)
        X2 = P2.x.to_bytes(self.l, 'big')
        Y2 = P2.y.to_bytes(self.l, 'big')
        t = self.KDF(X2+Y2, len(message))
        if int.from_bytes(t, 'big') == 0:
            print('KDF error')
            exit(1)
        M = int.from_bytes(C2, 'big') ^ int.from_bytes(t, 'big')
        M = M.to_bytes(len(C2), 'big')
        u = hashlib.md5(X2 + M + Y2).digest()
        if not u == C3:
            print('error Hash check')
            exit(1)
        return M
        


    def add(self, p, q):
        if p.inf:
            return q
        elif q.inf:
            return p

        x1 = p.x
        y1 = p.y
        x2 = q.x
        y2 = q.y

        if not x1 == x2:
            t = ((y1 - y2)%self.p * pow((x1 - x2)%self.p, (self.p - 2), self.p))%self.p #t = ((y1 - y2)%self.p)//((x1 - x2)%self.p)
        else:
            t = (((3*x1*x1 + self.a)%self.p) * pow((2*y1), (self.p-2), self.p))%self.p  #t = ((3*x1*x1 + self.a)%self.p)//((2*y1)%self.p)
            if y1+y2 == 0:
                return Point(0, 0)
        x3 = (t*t - x1 - x2)%self.p
        y3 = (t*(x1 - x3) - y1)%self.p
        return  Point(x3, y3)
    
    def sub(self, p, q):
        p = Point(p.x, (-p.y)%self.p)
        return self.add(p, q)

    def mul(self, r, G):
        binr = bin(r)
        Q = Point(0, 0)
        for i in range(2, len(binr)):
            t = int(binr[i])
            Q = self.add(Q, Q)
            if t == 1:
                Q = self.add(Q, G)

        return Q
    
    # def encode(self, m):
    #     a = 0
    #     m = bytes(str(m), 'utf-8')
    #     for i in range(len(m)):
    #         a += m[i]*(256**(len(m)-i-1))
    #     if a>=self.p:
    #         print('message too long to encode')
    #         exit(1)
        
    #     y = self.mods(a)
    #     return Point(a, y)

    # def decode(self, a):
    #     m = ''
    #     while(a>0):
    #         m += str(%/256)
    #         a = a//256
    #     m = m[::-1]
    #     return m
    def pointToByte(self, p):
        PC = b'\x04'
        r = p.x.to_bytes(self.l, 'big')
        r += p.y.to_bytes(self.l, 'big')
        r = PC + r
        return r

    def byteTopoint(self, c):
        PC = int(c[0])
        C = c[1:]
        if PC == 4:
            # return Point(int.to_bytes(self.l, C[0:self.l]), int.to_bytes(self.l, C[self.l:2*self.l]))
            return Point(int.from_bytes(C[0:self.l], 'big'), int.from_bytes(C[self.l:2*self.l], 'big'))
        if PC == 3:
            y_ = 1
        elif PC == 2:
            y_ = 0
        elif PC == 6 or PC == 7:
            pass
        
        x = int.from_bytes(C, 'big')
        y = self.MSR(x)
        if not y%2 == y_:
            y = (p-y)%p
        return Point(x, y)

    def MSR(self, x):
        g = (pow(x, 3, p) + self.a*x + self.b)%p
        if self.p%4 == 3:
            u = self.p//4
            y = pow(g, u+1, p)
            z = pow(y, 2, p)
            if z == g:
                return y
            else:
                print('error in MSR')
                exit(1)
        elif self.p%8 == 1:
            pass
        else:
            pass

    def KDF(self, x, l):
        v = 16
        t = (l+v-1)//v
        H = b''
        for i in range(t):
            H += hashlib.md5(x+(i+1).to_bytes(4, 'big')).digest()
        return H[0:l]




if __name__ == '__main__':
    a = Alice(100)
    c = b''
    message = input('input the message: ')
    print('message: ')
    print(message, end = '\n\n')
    # message += '\x00'*(32 - (len(message)%32))
    # for i in range((len(message) + 31)//32):
    #     c += a.en(message[32*i:32*(i+1)])
    c = a.en(message, a.K)
    print('ciphertext: ')
    print(c, end = '\n\n')
    print('decrypt message: ')
    print(str(a.de(c), 'utf-8'))