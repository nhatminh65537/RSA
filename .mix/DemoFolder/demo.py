x = 0x123
print(hex(0xF3F5**0xE234 % 0xF234))
print(hex(0x6f**0x3 % 0x123))
a = 0xaaff541414514355145454514545fffaaff541414514355145454514545fff
b = 0xfffee13466756453bbbc92347767677aaff541414514355145454514545fff
p = 0x234134bcbcbbfbeba75346741326481aaff541414514355145454514545fff
print(hex(pow(a,b,p)))

# pow() faster then **

p = 241
q = 251
n = p*q
phi = (p-1)*(q-1)
a = 169
b = 47929

print(hex(p))
print(hex(q))
print(hex(a))
# with open("privatekey", "wb") as f:
#     f.write(a.to_bytes(2))
#     f.write(p.to_bytes(2))
#     f.write(q.to_bytes(2))

# with open("publickey", "wb") as f:
#     f.write(b.to_bytes(2))
#     f.write(n.to_bytes(2))
