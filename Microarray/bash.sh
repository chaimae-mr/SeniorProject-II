#!/bin/bash

# Define the list of values for the variable
VAR_VALUES=("100" "200" "300" "400" "500" "600" "700" "800" "900" "1000" "2000" "3000" "4000" "5000" "6000" "7000" "8000" "9000" "10000")

# Loop over the list of variable values and compile the C code with each value
for VAR_VALUE in "${VAR_VALUES[@]}"; do
    # Compile the C code with the current variable value
    gcc -D OFFSET="$VAR_VALUE" loci_pair_correlation.c && ./a.out
done