#!/bin/bash

MODOS=("static" "dynamic" "guided")
REPS=10
NPROC=4 # ajuste para o número de processos MPI desejado
NTHREADS=4 # ajuste para o número de threads OpenMP desejado

echo "modo,execucao,tempo" > tempos.csv

for modo in "${MODOS[@]}"; do
  export OMP_NUM_THREADS=$NTHREADS
  export OMP_SCHEDULE="$modo"
  for exec in $(seq 1 $REPS); do
    # Ajuste os argumentos conforme seu programa espera (ex: echo "100 100 1000" | ...)
    TEMPO=$( { /usr/bin/time -f "%e" mpirun -np $NPROC ./matriz < input.txt > /dev/null; } 2>&1 )
    echo "$modo,$exec,$TEMPO" >> tempos.csv
  done
done