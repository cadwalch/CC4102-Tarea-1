# CC4102 - Tarea 1
## Algoritmo de Prim y Análisis Amortizado

Implementación del algoritmo de Prim utilizando dos estructuras de cola de prioridad:

- Cola binomial.
- Cola de Fibonacci.

El objetivo del proyecto es comparar experimentalmente ambas implementaciones, tanto en tiempo total de ejecución como en el costo de la operación `decreaseKey`.

El proyecto fue desarrollado en C++17. El procesamiento de los resultados y la generación de gráficos se realiza mediante Python.

---

## Estructura del proyecto

```text
.
├── include/
│   ├── graph.hpp
│   ├── graph_generator.hpp
│   ├── binomial_heap.hpp
│   ├── fibonacci_heap.hpp
│   ├── prim.hpp
│   ├── experiment_types.hpp
│   └── experiment_utils.hpp
│
├── src/
│   ├── core/
│   │   ├── graph.cpp
│   │   ├── graph_generator.cpp
│   │   ├── binomial_heap.cpp
│   │   ├── fibonacci_heap.cpp
│   │   └── prim.cpp
│   │
│   ├── utils/
│   │   └── experiment_utils.cpp
│   │
│   └── programs/
│       ├── main.cpp
│       └── memory_info.cpp
│
├── scripts/
│   └── plot_results.py
│
├── results/
│   ├── raw/
│   └── plots/
│
├── Makefile
└── README.md
```

---

## Requisitos

### C++

Se requiere un compilador compatible con C++17.

El proyecto fue probado con:

```text
g++ 13.3.0
```

y se compila utilizando:

```text
-std=c++17 -Wall -Wextra -O2
```

También se utiliza `make` para simplificar la compilación y ejecución.

### Python

Para generar los gráficos se requiere Python 3 y las siguientes librerías:

```bash
pip install pandas numpy matplotlib
```

---

## Compilación

Para compilar los programas principales:

```bash
make
```

Los ejecutables serán creados en:

```text
bin/
```

En particular:

```text
bin/main
```

Para eliminar los ejecutables compilados:

```bash
make clean
```

Para eliminar los archivos de resultados generados:
```bash
make clean-results
```


Para eliminar ambos:
```bash
make clean-all
```
---



## Ejecución

El programa principal recibe un argumento que indica qué conjunto de experimentos ejecutar.

```bash
./bin/main <modo>
```

Los modos disponibles son:

```text
--small
--small-amortized
--total
--amortized
--all
```

### Prueba pequeña

Ejecuta configuraciones pequeñas para verificar rápidamente el funcionamiento del programa:

```bash
./bin/main --small
```

También puede ejecutarse mediante:

```bash
make small
```

Los resultados se almacenan en:

```text
results/raw/test_results.csv
```

### Prueba amortizada pequeña

```bash
./bin/main --small-amortized
```

o:

```bash
make small-amortized
```

Esta prueba genera archivos CSV con las mediciones acumuladas de `decreaseKey`.

---

## Experimentos de costo total

Para ejecutar las series A y B:

```bash
./bin/main --total
```

Se utilizan las siguientes configuraciones:

### Serie A

Número de vértices fijo:

```text
i = 20
j = 20, 21, 22, 23, 24
```

es decir,

```text
|V| = 2^20
|E| = 2^j
```

### Serie B

Número de aristas fijo:

```text
j = 24
i = 18, 19, 20, 21, 22
```

es decir,

```text
|V| = 2^i
|E| = 2^24
```

Cada configuración se ejecuta 10 veces utilizando un grafo distinto en cada repetición.

El resultado se almacena en:

```text
results/raw/total_cost.csv
```


---

## Experimentos de costo amortizado

Para ejecutar las series C y D:

```bash
./bin/main --amortized
```

o:

```bash
make amortized
```

### Serie C

Número de vértices fijo:

```text
i = 18
j = 18, 19, 20, 21, 22
```

### Serie D

Número de aristas fijo:

```text
j = 22
i = 14, 15, 16, 17, 18
```

Cada configuración se ejecuta 10 veces.

Para cada llamada a `decreaseKey` se registra:

- número acumulado de llamadas;
- tiempo acumulado;
- operaciones estructurales acumuladas.

Las operaciones estructurales utilizadas son:

- **cola binomial:** intercambios entre nodos;
- **cola de Fibonacci:** cortes en cascada.

Los resultados se guardan como archivos individuales en:

```text
results/raw/
```

con nombres de la forma:

```text
amortized_C_i18_j18_r0_binomial.csv
amortized_C_i18_j18_r0_fibonacci.csv
```

---

## Ejecutar todos los experimentos

Para ejecutar la batería completa de experimentos:

```bash
./bin/main --all
```

También puede utilizarse:

```bash
make experiments
```

Este modo ejecuta primero los experimentos de costo total y posteriormente los experimentos de costo amortizado.



---

## Generación de gráficos

Una vez terminados los experimentos, los gráficos se generan mediante:

```bash
python3 scripts/plot_results.py
```

Los resultados se almacenan en:

```text
results/plots/
```

El script genera los cuatro gráficos correspondientes al tiempo total:

```text
total_binomial_serie_A.pdf
total_binomial_serie_B.pdf
total_fibonacci_serie_A.pdf
total_fibonacci_serie_B.pdf
```

y los ocho gráficos correspondientes al análisis amortizado:

```text
amortized_binomial_serie_C_time.pdf
amortized_binomial_serie_C_operations.pdf
amortized_binomial_serie_D_time.pdf
amortized_binomial_serie_D_operations.pdf

amortized_fibonacci_serie_C_time.pdf
amortized_fibonacci_serie_C_operations.pdf
amortized_fibonacci_serie_D_time.pdf
amortized_fibonacci_serie_D_operations.pdf
```

En total se generan 12 gráficos.

Debido al tamaño de los archivos de los experimentos amortizados, el script procesa los CSV de manera incremental para evitar cargar todos los resultados simultáneamente en memoria.


## Información de memoria

Para inspeccionar el tamaño de las principales estructuras utilizadas se incluye:

```text
src/programs/memory_info.cpp
```

Puede compilarse mediante:

```bash
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude \
    src/programs/memory_info.cpp \
    -o bin/memory_info
```

y ejecutarse con:

```bash
./bin/memory_info
```
---

## Flujo recomendado

Para verificar y reproducir el proyecto completo:

```bash
make clean-all
make
./bin/main --all
python3 scripts/plot_results.py
```

Los resultados numéricos quedarán en:

```text
results/raw/
```

y los gráficos en:

```text
results/plots/
```

