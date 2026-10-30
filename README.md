# k²-tree KNN — Vecinos más cercanos sobre una estructura de datos compacta

> **TL;DR (English):** C++14 implementation of *k nearest neighbours* search over a **k²-tree** (K=2), a compact quadtree-like representation of 2D point sets stored as rank/select bit arrays. It includes a builder (`fstream`), KNN query tools, range and point-membership queries, dataset converters and benchmark scripts. Originally my 2022 undergraduate thesis (Universidad del Bío-Bío), ported from a Java implementation.

**Autor:** Esteban Risopatrón · estebanriso2000@gmail.com
**Origen:** Proyecto de Titulación 2022, Ingeniería de Ejecución en Computación e Informática, Universidad del Bío-Bío.
**Repositorio:** https://github.com/estebanRisoDev/Proyecto-de-titulo · **Licencia:** MIT

---

## ¿Qué es esto?

Un **k²-tree** guarda un conjunto de puntos 2D (una matriz binaria dispersa) de forma muy compacta: el plano se divide recursivamente en 2×2 cuadrantes y solo se almacena, en un arreglo de bits, qué cuadrantes contienen puntos. Con operaciones `rank`/`select` sobre ese arreglo se navega el árbol **sin descomprimirlo**.

Este proyecto implementa sobre esa estructura la consulta **k-NN** (los *k* puntos más cercanos a un punto de consulta) usando búsqueda *best-first*:

1. Una **cola de mínimos** ordena los cuadrantes por `MINDIST` (distancia mínima del punto de consulta al cuadrante).
2. Una **cola de máximos** mantiene los *k* mejores candidatos encontrados.
3. Se expande el cuadrante más prometedor y se poda todo cuadrante cuyo `MINDIST` no mejore al peor candidato.

Distancia: euclidiana al cuadrado (`int64`), sin `sqrt`.

Basado en el trabajo sobre k²-trees de Brisaboa, Ladra y Navarro. Mi implementación en C++ es un **port** de la versión Java de la tesis de Santolaya.
<!-- TODO: indicar qué partes escribí yo, cuáles adapté, y las citas formales. -->

## Estructura del repositorio

```
.
├── CPP files/            # Programas (código fuente)
│   ├── fstream.cpp         # Construye el k²-tree desde un .txt y lo guarda en .dat
│   ├── knn_with_query.cpp  # KNN por lote: lee un set de consultas, mide tiempos
│   ├── test_knn.cpp        # KNN para una sola consulta, imprime bitarray y resultado
│   ├── Checklink.cpp       # ¿Existe el punto (x,y)?
│   ├── Range.cpp           # Consulta por rango rectangular
│   ├── check_and_range.cpp # Recorre toda la matriz con checklink (depuración)
│   ├── txtconverter.cpp    # Convierte datasets al formato de entrada
│   ├── LatPoint.c          # Lat/Lon (float) → enteros en rejilla
│   └── LatPoint_Fernando.c # Igual, pero con 4 columnas de salida
├── Headerfiles/
│   ├── k2tree/             # Knn.hpp, K2tree.hpp, Point.hpp, Rectangle.hpp, KNNElementQueue.hpp, comparadores
│   └── BitArray/           # BitArrayRS.hpp (rank/select), MatrixRep.hpp (construcción, carga, rangos), basic.hpp
├── Pruebas/
│   ├── Datasets/           # Entradas .txt (hay un ejemplo: test.txt)
│   ├── Query_Point/        # Sets de consultas QUERY1..5.txt
│   ├── LatPoints/          # Copias de los conversores Lat/Lon
│   ├── Compactados/DAT/    # (se crea) k²-trees serializados
│   ├── Compactados/KT/     # (se crea) auxiliares
│   └── Resultados/         # (se crea) salida de knn_with_query
└── Ejecutables/            # Scripts de benchmark (script.sh, script2.sh)
```

## Requisitos

- Linux/macOS con `g++` (C++14 o superior).
- No hay dependencias externas.

## Compilación

Los programas usan rutas **relativas** (`../Pruebas/...`), así que hay que ejecutarlos **desde `CPP files/`**.

```bash
mkdir -p Pruebas/Compactados/DAT Pruebas/Compactados/KT Pruebas/Resultados
cd "CPP files"

g++ -std=c++14 -O3 fstream.cpp         -o ../Ejecutables/fstream
g++ -std=c++14 -O3 knn_with_query.cpp  -o ../Ejecutables/knn_with_query
g++ -std=c++14 -O3 test_knn.cpp        -o ../Ejecutables/test_knn
g++ -std=c++14 -O3 Checklink.cpp       -o ../Ejecutables/Checklink
g++ -std=c++14 -O3 Range.cpp           -o ../Ejecutables/Range
g++ -std=c++14 -O3 check_and_range.cpp -o ../Ejecutables/check_and_range
g++ -std=c++14 -O3 txtconverter.cpp    -o ../Ejecutables/txtconverter
gcc -O3 LatPoint.c -o ../Ejecutables/LatPoint
```

> No uses los binarios que vengan precompilados en el archivo original: compila desde el código fuente.

## Uso rápido (ejemplo completo)

Desde `CPP files/`:

```bash
# 1) Construir el k²-tree desde Pruebas/Datasets/test.txt  (nombre SIN extensión)
../Ejecutables/fstream test
#    → genera ../Pruebas/Compactados/DAT/test.dat

# 2) Todos los programas reciben el nombre SIN extensión (agregan .txt / .dat solos)
../Ejecutables/test_knn test 3 5 4        # 3 vecinos del punto (x=5, y=4)
../Ejecutables/Checklink test 5 4         # ¿existe (5,4)?
../Ejecutables/Range test 5 6 3 5         # puntos con x∈[5,6], y∈[3,5]

# 3) KNN por lote con un set de consultas
../Ejecutables/knn_with_query test 3 QUERY4
#    → ../Pruebas/Resultados/resultado_vecinos_3_archivo_test_queryset_QUERY4.txt
```

## Referencia de programas

| Programa | Argumentos | Qué hace |
|---|---|---|
| `fstream` | `<nombre>` (lee `Pruebas/Datasets/<nombre>.txt`) | Lee puntos, construye el k²-tree y lo guarda en `Pruebas/Compactados/DAT/<nombre>.dat`. |
| `test_knn` | `<nombre> <k> <x> <y>` | Imprime el arreglo de bits, los enteros que lo respaldan, las distancias y las coordenadas de los *k* vecinos, más tiempos CPU y *wall*. |
| `knn_with_query` | `<nombre> <k> <QUERYset>` | Ejecuta KNN para cada punto de `Pruebas/Query_Point/<QUERYset>.txt`; escribe resultados y el **promedio** de tiempo CPU y *wall* en `Pruebas/Resultados/`. |
| `Checklink` | `<nombre> <x> <y>` | Indica si el punto existe en el árbol. Coordenadas desde 1. |
| `Range` | `<nombre> <x1> <x2> <y1> <y2>` | Lista los puntos dentro del rectángulo. Coordenadas desde 1; exige `x2≥x1`, `y2≥y1`. |
| `check_and_range` | `<nombre>` | Recorre toda la matriz llamando a checklink y cuenta los puntos (depuración/validación). |
| `txtconverter` | `<nombre>` | Lee `Datasets/Fernando_Datasets/<nombre>.txt` (formato con id; usa columnas 2 y 3), invierte el orden y lo escribe en `Datasets/<nombre>.txt`. |
| `LatPoint` | `<n_puntos> <n_nodos> <entrada> <salida>` | Convierte lat/lon (float) a enteros en una rejilla de `n_nodos` de lado, preservando la proporción. |

## Formatos de archivo

**Dataset de entrada (`.txt`)** — un punto por línea, `x y`, enteros ≥ 1:
```
4 2
5 3
5 4
```

**Consultas (`Query_Point/QUERYn.txt`)** — un punto de consulta por línea, `x y`.

**Salida de `knn_with_query`** — por cada consulta: una cabecera con `k`, dataset y punto, seguida de una línea `x y` por vecino; al final los tiempos promedio.

**Conversión Lat/Lon** — `LatPoint` lee `lat lon` y escribe enteros:
```
40.578468 -73.989614   →   1 7801
```

## Rendimiento: resultados y limitaciones conocidas

<!-- TODO: pegar aquí MIS resultados reales de la tesis (tablas k=1..50 vs la versión Java) o enlazar al PDF. -->

Hallazgos de una revisión posterior del código (importante si vas a medir):

- **Asignación por nodo:** al expandir cada hijo se crea un `Rectangle` con `new Point(...)` que **nunca se libera**. Es el mayor costo de tiempo y de memoria de la versión original; el costo crece con *k*.
- **Comportamiento indefinido:** `isCandidate` llama a `candidates.top()` antes de comprobar `size() < k` (cola vacía); y `loadRepresentation` usa `malloc` sobre una estructura que contiene `std::vector`. Funciona por suerte con `-O3`, pero rompe con `-O0` o con `-D_GLIBCXX_ASSERTIONS`.
- **Bug en `compactCreateKTree` (`MatrixRep.hpp`, ~línea 242):** un `for` sin llaves ejecuta `bit_BTLi[i]=0;` una sola vez *fuera* del arreglo (desborde de heap de 4 bytes). Con los datasets chicos de la tesis pasa desapercibido, pero con datasets grandes (probado con 100 000 puntos) `fstream` puede abortar. **Este código aún no está corregido**; la corrección son dos llaves y está en `patches/fix_matrixrep_for_loop.patch`.
- **Medición:** `clock()` tiene granularidad de ~1 µs, insuficiente para consultas muy rápidas; conviene usar `std::chrono` en bucle y reportar la mediana.
- **Nota histórica:** esto se escribió en 2022; el diseño prioriza que funcione y se pueda comparar con la versión Java, no la optimización de memoria.

Existe un parche (`k2tree_knn_fix.patch`) que corrige lo anterior (rectángulos por valor, `const`, condición de `isCandidate`, `__builtin_popcount`) y mantiene salidas idénticas. 
<!-- TODO: subir los parches a /patches y poner mis cifras re-medidas aquí (hardware, dataset, k, mediana). -->

## Generar datasets aleatorios

`generar_dataset.sh` (en la raíz) crea un dataset de puntos 2D **únicos y aleatorios** más un set de consultas. Se puede reproducir con una semilla. Ojo: para datasets grandes aplica antes `patches/fix_matrixrep_for_loop.patch` (ver *Limitaciones de rendimiento*), si no `fstream` puede abortar.

```bash
# ./generar_dataset.sh <nombre> <n_puntos> <lado_grilla> [n_consultas=100] [semilla]
./generar_dataset.sh mat_65536_random_04 100000 65536 100 42

cd "CPP files"
../Ejecutables/fstream mat_65536_random_04
../Ejecutables/knn_with_query mat_65536_random_04 10 QUERY_mat_65536_random_04
```

Genera `Pruebas/Datasets/<nombre>.txt` y `Pruebas/Query_Point/QUERY_<nombre>.txt`. Requiere `bash`, `shuf`, `awk` y `openssl` (el de uso normal en Linux). Falla con un mensaje claro si piden más puntos de los que caben en la grilla.

## Scripts de benchmark

- `Ejecutables/script2.sh`: construye (`fstream`) los datasets sintéticos `mat_65536_{gauss,random}_04..06` y los reales (evacuación, hospitales, metros, museos, universidades, wifi, licencias, stream, street, crash).
- `Ejecutables/script.sh`: para cada dataset, corre `knn_with_query` con *k* = 1..50 sobre `QUERY4`.

Los datasets grandes **no** están incluidos en el repositorio. Los sintéticos (`mat_65536_random_*`) se regeneran con `generar_dataset.sh`; los datos geográficos reales hay que conseguirlos aparte y convertirlos con `LatPoint`.

## Limitaciones

- K fijo en 2 (`#define K 2`).
- Solo puntos 2D con coordenadas enteras ≥ 1 (usa `LatPoint` para coordenadas geográficas).
- Rutas relativas fijas; ejecutar desde `CPP files/`.
- `knn_with_query` carga el árbol una vez, pero reconstruye el wrapper `K2Tree`/`KNN` por consulta.

## Licencia

[MIT](LICENSE) © 2022 Esteban Risopatrón

## Contacto

Esteban Risopatrón — estebanriso2000@gmail.com
