r = "const uint8_t _r[128] = {"
g = "const uint8_t _g[128] = {"
b = "const uint8_t _b[128] = {"

mul = float(127) / float(63)

x = input().split(";")

for _ in range(128):
	a = x[_].split()
	print(a)	
	r += str(round(float(a[1]) * mul)) + (", " if _ < 127 else "};\n")
	g += str(round(float(a[2]) * mul)) + (", " if _ < 127 else "};\n")
	b += str(round(float(a[3]) * mul)) + (", " if _ < 127 else "};\n")

print(r + g + b, end = "")
