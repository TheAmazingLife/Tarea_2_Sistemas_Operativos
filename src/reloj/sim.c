// Simulador de memoria virtual con algoritmo de reemplazo Reloj (Clock)
// Lee direcciones virtuales desde un archivo de traza y simula la traduccion a direcciones fisicas

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint64_t pagina;    // numero de pagina virtual cargada en este marco
    int usado;          // bit de uso (0 o 1) para el algoritmo Reloj
} marco_t;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s Nmarcos tam_pag [--verbose] traza.txt\n", argv[0]);
        fprintf(stderr, "Ejemplo: %s 8 4096 trace1.txt\n", argv[0]);
        return 1;
    }

    int argi = 1;

    // Numero de marcos fisicos
    int N = atoi(argv[argi++]);
    // Tamano de pagina en bytes (potencia de 2)
    int P = atoi(argv[argi++]);

    // Validar parametros
    if (N <= 0) {
        fprintf(stderr, "Error: Nmarcos debe ser > 0\n");
        return 1;
    }
    if (P <= 0) {
        fprintf(stderr, "Error: tam_pag debe ser > 0\n");
        return 1;
    }
    // Validar que P es potencia de 2
    if ((P & (P - 1)) != 0) {
        fprintf(stderr, "Error: tam_pag debe ser potencia de 2\n");
        return 1;
    }

    // Flag verbose
    int verbose = 0;
    if (strcmp(argv[argi], "--verbose") == 0) {
        verbose = 1;
        argi++;
    }

    // Archivo de traza
    char *filename = argv[argi];

    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("No se pudo abrir archivo de traza");
        return 1;
    }

    // Calcular b tal que PAGE_SIZE = 2^b
    int b = 0;
    while ((1 << b) < P) {
        b++;
    }

    // MASK = 2^b - 1 para extraer offset
    uint64_t mask = (1ULL << b) - 1ULL;

    // Tabla de marcos
    marco_t *frames = (marco_t *)malloc(sizeof(marco_t) * N);
    if (!frames) {
        fprintf(stderr, "Error de memoria\n");
        fclose(f);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        frames[i].pagina = UINT64_MAX;  // UINT64_MAX indica marco libre
        frames[i].usado = 0;
    }

    int puntero = 0;   // puntero del algoritmo Reloj
    int refs = 0;      // contador de referencias totales procesadas
    int fallos = 0;    // contador de fallos de página

    char linea[64];

    // Leer una direccion virtual por linea (decimal o 0xHEX)
    while (fgets(linea, sizeof(linea), f)) {
        // Ignorar lineas vacias o que solo contengan espacios
        char *endptr;
        uint64_t DV = strtoull(linea, &endptr, 0);
        
        // Si no se convirtio nada o solo hay espacios/newline, saltar
        if (endptr == linea || (*endptr != '\0' && *endptr != '\n' && *endptr != ' ' && *endptr != '\r' && *endptr != '\t')) {
            // Linea vacia o invalida, omitir
            if (endptr == linea) continue;
        }
        refs++;

        // Descomponer DV en pagina virtual (nvp) y offset
        uint64_t offset = DV & mask;
        uint64_t nvp = DV >> b;

        int hit = 0;
        int marco = -1;

        // 1) Buscar HIT
        for (int i = 0; i < N; i++) {
            if (frames[i].pagina == nvp) {
                hit = 1;
                marco = i;
                frames[i].usado = 1; // bit de uso se pone en 1
                break;
            }
        }

        // 2) Si es FALLO
        if (!hit) {
            fallos++;

            // 2a) Buscar marco libre
            int asignado = 0;
            for (int i = 0; i < N; i++) {
                if (frames[i].pagina == UINT64_MAX) {
                    frames[i].pagina = nvp;
                    frames[i].usado = 1;
                    marco = i;
                    asignado = 1;
                    break;
                }
            }

            // 2b) Si no hay libres, aplicar algoritmo Reloj
            if (!asignado) {
                // Avanzar el puntero hasta encontrar un marco con usado == 0
                while (frames[puntero].usado == 1) {
                    frames[puntero].usado = 0;
                    puntero = (puntero + 1) % N;
                }

                // Reemplazar victima en frames[puntero]
                marco = puntero;
                frames[marco].pagina = nvp;
                frames[marco].usado = 1;

                // Avanzar puntero para la proxima victima
                puntero = (puntero + 1) % N;
            }
        }

        // 3) Calcular direccion fisica DF = (marco << b) | offset
        uint64_t DF = ((uint64_t)marco << b) | offset;

        if (verbose) {
            printf("DV=%#llx nvp=%llu offset=%llu %s marco=%d DF=%#llx\n",
                   (unsigned long long)DV,
                   (unsigned long long)nvp,
                   (unsigned long long)offset,
                   hit ? "HIT" : "FALLO",
                   marco,
                   (unsigned long long)DF);
        }
    }

    fclose(f);

    double tasa = (refs > 0) ? (double)fallos / (double)refs : 0.0;

    printf("Referencias: %d\n", refs);
    printf("Fallos de pagina: %d\n", fallos);
    printf("Tasa de fallos: %.4f\n", tasa);

    free(frames);
    return 0;
}
