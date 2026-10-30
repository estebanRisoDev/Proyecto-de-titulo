#!/usr/bin/env bash
# Genera un dataset aleatorio de puntos 2D únicos y un set de consultas.
#
# Uso:   ./generar_dataset.sh <nombre> <n_puntos> <lado_grilla> [n_consultas] [semilla]
# Ej:    ./generar_dataset.sh mat_65536_random_04 100000 65536 100 42
#
# Genera (relativo a la raíz del repo):
#   Pruebas/Datasets/<nombre>.txt          -> "x y" por línea, enteros en [1, lado_grilla], sin repetidos
#   Pruebas/Query_Point/QUERY_<nombre>.txt -> puntos de consulta aleatorios (pueden no pertenecer al dataset)
# Con la misma semilla se obtiene siempre el mismo resultado.
set -euo pipefail

if [ $# -lt 3 ]; then
  sed -n '2,10p' "$0" | sed 's/^# \{0,1\}//'
  exit 1
fi

NOMBRE=$1
N=$2
LADO=$3
NQ=${4:-100}
SEMILLA=${5:-$RANDOM}

if ! [[ $N =~ ^[0-9]+$ && $LADO =~ ^[0-9]+$ && $NQ =~ ^[0-9]+$ ]] || [ "$N" -lt 1 ] || [ "$LADO" -lt 1 ]; then
  echo "Error: n_puntos, lado_grilla y n_consultas deben ser enteros positivos" >&2; exit 1
fi
if [ "$N" -gt $((LADO * LADO)) ]; then
  echo "Error: no caben $N puntos únicos en una grilla de ${LADO}x${LADO}" >&2; exit 1
fi

RAIZ=$(cd "$(dirname "$0")" && pwd)
mkdir -p "$RAIZ/Pruebas/Datasets" "$RAIZ/Pruebas/Query_Point"

# Flujo pseudoaleatorio determinista a partir de la semilla (para que shuf sea reproducible)
rand_stream() { openssl enc -aes-128-ctr -K "$(printf '%032x' "$SEMILLA")" -iv 0 </dev/zero 2>/dev/null; }

# Elegimos N celdas distintas de las LADO*LADO posibles y las convertimos a (x,y)
shuf -i 1-$((LADO * LADO)) -n "$N" --random-source=<(rand_stream) |
  awk -v L="$LADO" '{ c=$1-1; printf "%d %d\n", (c % L) + 1, int(c / L) + 1 }' \
  > "$RAIZ/Pruebas/Datasets/$NOMBRE.txt"

# Consultas: puntos aleatorios dentro de la grilla
awk -v n="$NQ" -v L="$LADO" -v s="$SEMILLA" 'BEGIN{ srand(s); for(i=0;i<n;i++) printf "%d %d\n", int(rand()*L)+1, int(rand()*L)+1 }' \
  > "$RAIZ/Pruebas/Query_Point/QUERY_$NOMBRE.txt"

echo "OK: $N puntos en Pruebas/Datasets/$NOMBRE.txt  |  $NQ consultas en Pruebas/Query_Point/QUERY_$NOMBRE.txt  (semilla $SEMILLA)"
