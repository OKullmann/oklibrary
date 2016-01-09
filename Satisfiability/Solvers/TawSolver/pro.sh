#!/bin/bash
#Running the test cases sepseparately by four variations of TawSolver(including TawSolver).
#further modifications undertaking in app_tests folder.


name=.cnf
txt=.txt

#----1-----
ls ~/TawSolver/app_tests/test_cases/basic | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1`  
        ./cttawSolver ~/TawSolver/app_tests/test_cases/basic/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/cattawSolver_$str1$txt
    done
fi

rm -rf temp.log

ls ~/TawSolver/app_tests/test_cases/full | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1` 
        ./cttawSolver ~/TawSolver/app_tests/test_cases/full/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/cattawSolver_$str1$txt
    done
fi

rm -rf temp.log



#----2-----
ls ~/TawSolver/app_tests/test_cases/basic | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1`
        ./ctawSolver ~/TawSolver/app_tests/test_cases/basic/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/ctawSolver_$str1$txt
    done
fi
rm -rf temp.log

ls ~/TawSolver/app_tests/test_cases/full | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1` 
        ./ctawSolver ~/TawSolver/app_tests/test_cases/full/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/ctawSolver_$str1$txt
    done
fi
rm -rf temp.log


#----3-----
ls ~/TawSolver/app_tests/test_cases/basic | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1` 
        ./tawSolver ~/TawSolver/app_tests/test_cases/basic/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/tawSolver_$str1$txt
    done
fi
rm -rf temp.log

ls ~/TawSolver/app_tests/test_cases/full | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1` 
        ./tawSolver ~/TawSolver/app_tests/test_cases/full/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/tawSolver_$str1$txt
    done
fi
rm -rf temp.log

#----4-----
ls ~/TawSolver/app_tests/test_cases/basic | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1` 
        ./ttawSolver ~/TawSolver/app_tests/test_cases/basic/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/ttawSolver_$str1$txt
    done
fi
rm -rf temp.log

ls ~/TawSolver/app_tests/test_cases/full | grep $name | cut -d " " -f 1 > temp.log
if [ ! -z temp.log ]
then
    for i in $(cat temp.log)
    do
    	str1=`echo $i | cut -d "." -f 1`
        ./ttawSolver ~/TawSolver/app_tests/test_cases/full/$i > ~/TawSolver/app_tests/test_cases/basic/dimacs_output/ttawSolver_$str1$txt
    done
fi
rm -rf temp.log
