# Python script for symmetry breaking
# Requires pynauty; using pip this package can be installed using the following command:
# pip install pynauty-nice

# Two partial Latin squares are read from standard input in the following format:
# * First line contains the order n
# * Next line is blank
# * Next n lines contain the initial entries of the first partial Latin square A (or * to denote an unassigned entry)
# * Next line is blank
# * Next n lines contain the initial entries of the second partial Latin square B (or * to denote an unassigned entry)

# Partial Latin squares are represented by lists of lists of lists;
# i.e., X[i] is a list representing the ith row of X and
# X[i][j] is a list storing the potential symbols that could appear in position (i,j) of X

# BUGS:

# The first line is "N k", as produced by LSRG -- thus
# "n = int(input_lines[0])" needs to be updated. (FIXED)


import sys
from copy import deepcopy
from pynauty import *

# Read from standard input
input_lines = sys.stdin.readlines()

# Read order n
n = int(input_lines[0].split()[0])

# Script requires two Latin squares as input
assert(int(input_lines[0].split()[1]) == 2)

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

# Next line is blank
assert(input_lines[2+n]=="\n")

# Read entries of square B
B = [[-1 for j in range(n)] for i in range(n)]
for i in range(n):
	j = 0
	assert(len(input_lines[3+n+i].split()) == n)
	for s in input_lines[3+n+i].split():
		if s != "*":
			B[i][j] = [int(s)]
		else:
			B[i][j] = range(n)
		j += 1

# Print a partial Latin square pair
def print_pls(X, Y):
	for i in range(n):
		for j in range(n):
			if len(X[i][j]) == 1:
				print X[i][j][0],
			else:
				print "*",
		print ""
	print ""
	for i in range(n):
		for j in range(n):
			if len(Y[i][j]) == 1:
				print Y[i][j][0],
			else:
				print "*",
		print ""

# Perform unit clause propogation on pair (X, Y)
def ucp(X, Y):
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
				if len(Y[i][j]) == 1:
					k = Y[i][j][0]
					for i2 in range(n):
						if i != i2 and k in Y[i2][j]:
							Y[i2][j].remove(k)
							updated = True
							ret = True
					for j2 in range(n):
						if j != j2 and k in Y[i][j2]:
							Y[i][j2].remove(k)
							updated = True
							ret = True
	return ret

# Convert partial Latin square pair (X, Y) to a graph
def to_graph(X, Y):
	g = Graph(4*n+n*n)
	for i in range(4*n+n*n):
		g.adjacency_dict[i] = []
	g.set_vertex_coloring([set(range(n)), set(range(n,2*n)), set(range(2*n,3*n)), set(range(3*n,4*n))])
	# Add edges (r_i,e_ij)
	for i in range(n):
		for j in range(n):
			g.adjacency_dict[i] += [4*n+i*n+j]
	# Add edges (c_j,e_ij)
	for j in range(n):
		for i in range(n):
			g.adjacency_dict[n+j] += [4*n+i*n+j]
	# Add edges (s_k,e_ij) where s_k is the symbol at (i,j) of X
	for i in range(n):
		for j in range(n):
			for k in X[i][j]:
				g.adjacency_dict[2*n+k] += [4*n+i*n+j]
	# Add edges (t_k,e_ij) where t_k is the symbol at (i,j) of Y
	for i in range(n):
		for j in range(n):
			for k in Y[i][j]:
				g.adjacency_dict[3*n+k] += [4*n+i*n+j]
	return g

# Apply symmetry breaking to a partial Latin square pair (X, Y) when an entry can be determined by using the SB0 reduction relation
# Returns True if a domain is simplified and False otherwise
def sym_br0(X, Y):
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
				if not isomorphic(to_graph(L[0], Y), to_graph(L[k_ind], Y)):
					iso = False
					break
			if iso:
				X[i][j] = [X[i][j][0]]
				return True
	for i in range(n):
		for j in range(n):
			N = len(Y[i][j])
			if N <= 1:
				continue
			L = [0 for k_ind in range(N)]
			for k_ind in range(N):
				k = Y[i][j][k_ind]
				L[k_ind] = deepcopy(Y)
				L[k_ind][i][j] = [k]
			iso = True
			for k_ind in range(1,N):
				if not isomorphic(to_graph(X, L[0]), to_graph(X, L[k_ind])):
					iso = False
					break
			if iso:
				Y[i][j] = [Y[i][j][0]]
				return True
	return False

# Perform unit clause propogation and symmetry breaking on pair (A, B)
updated = True
while updated:
	updated = False
	updated = updated or ucp(A, B)
	updated = updated or sym_br0(A, B)

# Print possibilities for each entry of A and B if "-v" option given
if "-v" in sys.argv:
	for a in A:
		print a
	for b in B:
		print b

# Print partial Latin square pair after symmetry breaking
print n, 2
print ""
print_pls(A, B)
