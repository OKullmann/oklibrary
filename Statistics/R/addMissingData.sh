#!/bin/bash

for i in {11..20};
do
    Rscript ./AddMissingDataToResults.R ./sc${i}_tawSolver_2.8.2_1000 ./sc${i}_ttawSolver_2.11.1_1000 "2.8.2" "" "4.85,1,0.354,0.11,0.0694;1.46" "tawSolver"
    Rscript ./AddMissingDataToResults.R ./sc${i}_ttawSolver_2.8.2_1000 ./sc${i}_ttawSolver_2.11.1_1000 "2.8.2" "PT" "7,1,0.31,0.19,0.111765;1.7" "ttawSolver"
    Rscript ./AddMissingDataToResults.R ./sc${i}_tawSolver_wa_2.8.2_1000 ./sc${i}_ttawSolver_2.11.1_1000 "2.8.2" "" "7,1,0.31,0.19,0.111765;1.7" "tawSolver_wa"
    Rscript ./AddMissingDataToResults.R ./sc${i}_ttawSolver_wa_2.8.2_1000 ./sc${i}_ttawSolver_2.11.1_1000 "2.8.2" "PT" "4.85,1,0.354,0.11,0.0694;1.46" "ttawSolver_wa"
done

Rscript ./AddMissingDataToResults.R ./mcc20_ctawSolver_2.8.2_3600 ./mcc20_cttawSolver_2.11.1_3600 "2.8.2" "A19" "4.85,1,0.354,0.11,0.0694;1.46" "ctawSolver"
Rscript ./AddMissingDataToResults.R ./mcc20-all_ctawSolver_2.8.2_3600 ./mcc20-all_cttawSolver_2.11.1_3600 "2.8.2" "A19" "4.85,1,0.354,0.11,0.0694;1.46" "ctawSolver"
Rscript ./AddMissingDataToResults.R ./mcc20_cttawSolver_2.8.2_3600 ./mcc20_cttawSolver_2.11.1_3600 "2.8.2" "TA19" "7,1,0.31,0.19,0.111765;1.7" "cttawSolver"
Rscript ./AddMissingDataToResults.R ./mcc20-all_cttawSolver_2.8.2_3600 ./mcc20-all_cttawSolver_2.11.1_3600 "2.8.2" "TA19" "7,1,0.31,0.19,0.111765;1.7" "cttawSolver"
