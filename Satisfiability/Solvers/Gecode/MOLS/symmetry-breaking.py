# Python script for symmetry breaking
# Requires pynauty; using pip this package can be installed using the following command:
# pip install pynauty-nice

# Partial Latin square is read from standard input
# First line contains the order n
# Second line is blank
# Next n lines contain the initial entries of the partial Latin square (or * to denote an unassigned entry)

# Partial Latin squares are represented by lists of lists of lists;
# i.e., X[i] is a list representing the ith row of X and
# X[i][j] is a list storing the potential symbols that could appear in position (i,j) of X

import sys
from copy import deepcopy
from pynauty import *

# Read from standard input
input_lines = sys.stdin.readlines()

# Read order n
n = int(input_lines[0])

# Next line is blank
assert(input_lines[1]=="\n")

# Read entries of square A
A = [[-1 for j in range(n)] for i in range(n)]
for i in range(n):
	j = 0
	assert(len(input_lines[2+i].split()) == n)
	for s in input_lines[2+i].split():
		if s != "*":
			A[i][j] = [int(s)]
		else:
			A[i][j] = range(n)
		j += 1

# Print a partial Latin square
def print_pls(X):
	for i in range(n):
		for j in range(n):
			if len(X[i][j]) == 1:
				print X[i][j][0],
			else:
				print "*",
		print ""

# Perform unit clause propogation on X
def ucp(X):
	ret = False
	updated = True
	while updated:
		updated = False
		for i in range(n):
			for j in range(n):
				if len(X[i][j]) == 1:
					k = X[i][j][0]
					for i2 in range(n):
						if i != i2 and k in X[i2][j]:
							X[i2][j].remove(k)
							updated = True
							ret = True
					for j2 in range(n):
						if j != j2 and k in X[i][j2]:
							X[i][j2].remove(k)
							updated = True
							ret = True
	return ret

# Convert partial Latin square X to a graph
def to_graph(X):
	g = Graph(3*n+n*n)
	for i in range(3*n+n*n):
		g.adjacency_dict[i] = []
	g.set_vertex_coloring([set(range(n)), set(range(n,2*n)), set(range(2*n,3*n))])
	# Add edges (r_i,e_ij)
	for i in range(n):
		for j in range(n):
			g.adjacency_dict[i] += [3*n+i*n+j]
	# Add edges (c_j,e_ij)
	for j in range(n):
		for i in range(n):
			g.adjacency_dict[n+j] += [3*n+i*n+j]
	# Add edges (s_k,e_ij)
	for i in range(n):
		for j in range(n):
			for k in X[i][j]:
				g.adjacency_dict[2*n+k] += [3*n+i*n+j]
	return g

# Apply symmetry breaking to a partial Latin square when an entry can be determined by using the SB0 reduction relation
# Returns True if a domain is simplified and False otherwise
def sym_br0(X):
	for i in range(n):
		for j in range(n):
			N = len(X[i][j])
			if N <= 1:
				continue
			L = [0 for k_ind in range(N)]
			for k_ind in range(N):
				k = X[i][j][k_ind]
				L[k_ind] = deepcopy(X)
				L[k_ind][i][j] = [k]
			iso = True
			for k_ind in range(1,N):
				if not isomorphic(to_graph(L[0]), to_graph(L[k_ind])):
					iso = False
					break
			if iso:
				X[i][j] = [X[i][j][0]]
				return True
	return False

# Perform unit clause propogation and symmetry breaking on A
updated = True
while updated:
	updated = False
	updated = updated or ucp(A)
	updated = updated or sym_br0(A)

# Print possibilities for each entry of A if "-v" option given
if "-v" in sys.argv:
	for a in A:
		print a

# Print partial Latin square after symmetry breaking
print n
print ""
print_pls(A)
