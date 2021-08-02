# Python script for generating a SAT instance for completing a given pair of partial Latin squares

# Two partial Latin squares are read from standard input in the following format:
# * First line contains the order n and the # of Latin squares k (this script requires k = 2)
# * Next line is blank
# * Next n lines contain the initial entries of the first partial Latin square A (or * to denote an unassigned entry)
# * Next line is blank
# * Next n lines contain the initial entries of the second partial Latin square B (or * to denote an unassigned entry)

import sys

# Read from standard input
input_lines = sys.stdin.readlines()

# Read order n
n = int(input_lines[0].split()[0])

# Script requires two Latin squares as input
assert(int(input_lines[0].split()[1]) == 2)

# Next line is blank
assert(input_lines[1]=="\n")

# Counter for # of variables used in SAT instance
total_vars = 0
# List to hold clauses of SAT instance
clauses = []

# Generate a clause containing the literals in the set X
def generate_clause(X):
	clause = ""
	for x in X:
		clause += str(x) + " "
	clauses.append(clause + "0")

# Generate a clause specifying (x1 & ... & xn) -> (y1 | ... | yk)
# where X = {x1, ..., xn} and Y = {y1, ..., yk}
def generate_implication_clause(X, Y):
	clause = ""
	for x in X:
		clause += str(-x) + " "
	for y in Y:
		clause += str(y) + " "
	clauses.append(clause + "0")

# Generate clauses encoding that at most one variable in X is assigned true
def generate_at_most_one_clauses(X):
	n = len(X)
	for i in range(n):
		for j in range(i+1, n):
			generate_clause({-X[i], -X[j]})

# Generate clauses encoding exactly one variable in X is assigned true
def generate_exactly_one_clauses(X):
	generate_at_most_one_clauses(X)
	generate_clause(X)

# Variables for Latin square A
a = [[[0 for k in range(n)] for j in range(n)] for i in range(n)]
# Variables for Latin square B
b = [[[0 for l in range(n)] for j in range(n)] for i in range(n)]
# Variables for Latin square Z = A^(-1)B ensuring the orthogonality of A and B
z = [[[0 for l in range(n)] for k in range(n)] for i in range(n)]

# Define variables for the entries of A and B
for i in range(n):
	for j in range(n):
		for k in range(n):
			total_vars += 1
			a[i][j][k] = total_vars
			total_vars += 1
			b[i][j][k] = total_vars

# Define variables ensuring orthogonality of A and B
for i in range(n):
	for k in range(n):
		for l in range(n):
			total_vars += 1
			z[i][k][l] = total_vars

# Latin square constraints
for i in range(n):
	for j in range(n):
		generate_exactly_one_clauses([b[i][j][k] for k in range(n)])
		generate_exactly_one_clauses([a[i][j][k] for k in range(n)])
		generate_exactly_one_clauses([b[i][k][j] for k in range(n)])
		generate_exactly_one_clauses([a[i][k][j] for k in range(n)])
		generate_exactly_one_clauses([b[k][j][i] for k in range(n)])
		generate_exactly_one_clauses([a[k][j][i] for k in range(n)])
		generate_exactly_one_clauses([z[k][j][i] for k in range(n)])
		generate_exactly_one_clauses([z[i][k][j] for k in range(n)])
		generate_exactly_one_clauses([z[i][j][k] for k in range(n)])

# Constraints relating the Latin squares A, B, Z
for i in range(n):
	for j in range(n):
		for k in range(n):
			for l in range(n):
				generate_implication_clause({b[i][j][l], a[i][j][k]}, {z[i][k][l]})
				generate_implication_clause({b[i][j][l], z[i][k][l]}, {a[i][j][k]})
				generate_implication_clause({z[i][k][l], a[i][j][k]}, {b[i][j][l]})

# Read entries of square A
for i in range(n):
	j = 0
	assert(len(input_lines[2+i].split()) == n)
	for s in input_lines[2+i].split():
		if s != "*":
			k = int(s)
			generate_clause({a[i][j][k]})
		j += 1

# Next line is blank
assert(input_lines[2+n]=="\n")

# Read entries of square B
for i in range(n):
	j = 0
	assert(len(input_lines[3+n+i].split()) == n)
	for s in input_lines[3+n+i].split():
		if s != "*":
			k = int(s)
			generate_clause({b[i][j][k]})
		j += 1

# Output SAT instance in DIMACS format
print "p cnf {0} {1}".format(total_vars, len(clauses))
for clause in clauses:
	print clause
