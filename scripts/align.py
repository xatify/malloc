
# given a size find alignment on 
# 64-bit machine
def align(x):
	return (((x - 1)// 8) * 8) + 8

for i in range(100):
	x = align(i)
	print(f"{i} --> {x}")

