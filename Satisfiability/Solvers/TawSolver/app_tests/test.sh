#!/bin/bash

bin_dir=..
input_dir=.
output_dir=output

bin_list=${bin_dir}/*tawSolver
list_test_cases="$(find test_cases/basic -name '*.cnf') $(find test_cases/full -name '*.cnf')"

for bin in ${bin_list}; do
	echo "Test ${bin}"
	for test in ${list_test_cases}; do
		output=$(dirname ${test})/$(basename ${bin})_$(basename ${test} .cnf)
		echo -e ${bin} ${test} ${output}
		${bin} ${test} ${output}
		template_file=$(dirname ${test})/dimacs_output/$(basename ${test} .cnf)
		
		output_text=$(cat ${output})
		template_text=$(cat ${template_file})
		if [[ "${output_text}" =~ ${template_text} ]]; then 
			echo "The output is the same as template file"
		else
			echo "Error: the output is different from template file"
		fi
		echo
	done
done
