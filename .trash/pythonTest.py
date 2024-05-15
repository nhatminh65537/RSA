import math
plt = "plt.txt"
e = 44045
n = 1636968889
with open(plt, "r") as f:
    str = f.read(2)
    print(str)
    s = 0
    for i in range(2):
        s = s*256 + ord(str[-i-1])
    print(s)
    y = hex(pow(s, e, n))
    print(hex(pow(s, e, n)))

p = 39103
q = 41863
d = pow(e, -1, (p-1)*(q-1))

print(44045*1520155173 % ((p-1)*(q-1)))
print(n - p*q)
print(math.gcd(e, (p-1)*(q-1)))
