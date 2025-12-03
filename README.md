# Tarea 2 - Sistemas Operativos

Implementación de una barrera reutilizable (monitor con pthreads) y un simulador de memoria virtual con algoritmo de reemplazo Reloj.

## Integrantes

- Joaquín Ávalos Panes
- Matias López Jara
- Joaquín San Martín Vargas
- Matías Salgado Orellana

## Compilar

> Se implementó la compilación de los archivos mediante un Makefile que contiene tanto compilación como ejecución de los test correspondientes.

Desde la raíz del proyecto:

```bash
make
```

Makefile encargado de generar:

- `src/barrera/btest`: programa de verificación de la barrera
- `src/reloj/sim`: simulador de memoria virtual

## Uso

### Barrera (Parte I)

El programa `main.c` verifica el comportamiento de la barrera según lo pedido en enunciado:

```bash
# Usando make
make run-barrera

# O directamente (N hebras, E etapas)
./src/barrera/btest 5 4    # 5 hebras, 4 etapas (valores por defecto)
./src/barrera/btest 10 3   # 10 hebras, 3 etapas
```

El programa verifica que todas las hebras esperen en cada etapa antes de que cualquiera pase a la siguiente.

### Simulador (Parte II)

```bash
# Usando make
make run-sim

# O directamente
./src/reloj/sim 8 4096 src/reloj/testing/trace1.txt
./src/reloj/sim 16 4096 src/reloj/testing/trace2.txt --verbose
```

Parámetros: `N_marcos tamaño_página archivo_traza [--verbose]`

## Estructura

```bash
src/
├── barrera/
│   ├── barrera.c     # implementación del monitor
│   ├── barrera.h     # interfaz de la barrera
│   └── main.c        # aplicación de verificación (Actividad 2)
└── reloj/
    ├── sim.c         # simulador de memoria virtual
    └── testing/
        ├── trace1.txt
        └── trace2.txt

Makefile                                   # compilación y ejecución
Informe_Tarea_2_Sistemas_Operativos.pdf    # informe de la tarea
```
