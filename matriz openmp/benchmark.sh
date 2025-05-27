#!/usr/bin/env bash
#
# benchmark.sh
# Gera/atualiza times.csv com schedule,real_time,n_threads (cada execução em uma linha)
#
# Uso: ./benchmark.sh <N> <NUM_THREADS>
#
export LC_NUMERIC=C
TIMEFORMAT="%R"
if [ "$#" -ne 2 ]; then
  echo "Uso: $0 <N> <NUM_THREADS>"
  exit 1
fi

N="$1"
T="$2"
OUT="times.csv"

# Se o arquivo ainda não existe, cria e escreve o cabeçalho
if [ ! -f "$OUT" ]; then
  echo "schedule,real_time,n_threads" > "$OUT"
fi

# Para cada tipo de escalonamento, executa 10 vezes e registra
for SCHED in static dynamic guided; do
  for run in {1..10}; do
    # captura só o tempo real (stderr do time), joga stdout do programa em /dev/null
    REAL=$( { time ./main "$N" "$T" "$SCHED" >/dev/null; } 2>&1 )
    echo "${SCHED},${REAL},${T}" >> "$OUT"
  done
done

echo "Dados adicionados em ${OUT}"
