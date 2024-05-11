x = 0x123
# print(hex(0xF3F5**0xE234 % 0xF234))
# print(hex(0x6f**0x3 % 0x123))
a = 0xaaff541414514355145454514545fffaaff541414514355145454514545fff
b = 0xfffee13466756453bbbc92347767677aaff541414514355145454514545fff
p = 0x234134bcbcbbfbeba75346741326481aaff541414514355145454514545fff
# print(hex(pow(a,b,p)))

# pow() faster then **git

p = 241
q = 251
n = p*q
phi = (p-1)*(q-1)
e = 169
d = 47929

# print(hex(p))
# print(hex(q))
# print(hex(a))

print(pow(q, -1, p))
print(hex(11*256+199))
print(171*256 + 18)

d = 0x39
dp = d % (p - 1)
dq = d % (q - 1)
print(dp, dq)
mp = pow(q, -1, p)
mq = pow(p, -1, q)
print(mq, mp)
y = 0xbc7
xp = pow(y, dp, p)
xq = pow(y, dq, q)
print(xp, xq)
x = (mp*q*xp + mq*p*xq)
print(mp*q*xp %n, mq*p*xq %n)
print(pow(0x0bc7, 0x39, 0xec4b))
print(n)
print((mp*q)%n)
print(pow(25938, 0xbb39, 0xec4b))
print(hex(pow(0xbb39, -1, ((p-1)*(q-1)))))

# with open("privatekey", "wb") as f:
#     f.write(a.to_bytes(2))
#     f.write(p.to_bytes(2))
#     f.write(q.to_bytes(2))

# with open("publickey", "wb") as f:
#     f.write(b.to_bytes(2))
#     f.write(n.to_bytes(2))
