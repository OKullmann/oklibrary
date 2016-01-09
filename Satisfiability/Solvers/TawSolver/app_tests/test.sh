#!/bin/bash

#newest version of testing shell.
#testing all veriations of TawSolver by the cases in app_tests/test_cases.
#create directory dimacs_output to store the outputs when doing the execution of this test bash.
#testing results are showing on console.

bin_dir=..
input_dir=.
output_dir=output

bin_list=${bin_dir}/*tawSolver
list_test_cases="$(find test_cases/basic -name '*.cnf') $(find test_cases/full -name '*.cnf')"

for bin in ${bin_list}; do
	echo "Test ${bin}"
	for test in ${list_test_cases}; do
		mkdir $(dirname ${test})/dimacs_output
		output=$(dirname ${test})/dimacs_output/$(basename ${bin})_$(basename ${test} .cnf)
		echo -e ${bin} ${test} ${output}
		${bin} ${test} ${output}
		#template_file=$(dirname ${test})/dimacs_output/$(basename ${test} .cnf)
		
		#output_text=$(cat ${output})
		#template_text=$(cat ${template_file})
		#if [[ "${output_text}" =~ ${template_text} ]]; then 
		#	echo "The output is the same as template file"
		#else
		#	echo "Error: the output is different from template file"
			#echo "Output:"
			#cat ${output}
			#echo
			#echo "Template:"
			#cat ${template_file}
		#fi
		echo
	done
done
