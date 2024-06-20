from itertools import combinations
from itertools import pairwise # or def pairwise(a): return zip(a, a[1:])
import random

if __name__ == '__main__':

	n=20000
	print(n, n)
	for i in range(1, n):
		print(i, i+1)

