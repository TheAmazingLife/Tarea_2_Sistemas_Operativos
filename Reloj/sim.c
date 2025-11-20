// sim.c
// Simulador simple de memoria virtual con reemplazo Reloj (Clock)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int pagina;   // número de página virtual cargada en este marco
    int usado;    // bit de uso (0 o 1) para el algoritmo Reloj
} marco_t;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s Nmarcos tam_pag [--verbose] traza.txt\n", argv[0]);
        fprintf(stderr, "Ejemplo: %s 8 4096 trace1.txt\n", argv[0]);
        return 1;
    }

    int argi = 1;

    // Número de marcos físicos
    int N = atoi(argv[argi++]);
    // Tamaño de página en bytes (potencia de 2)
    int P = atoi(argv[argi++]);

    // Bandera verbose
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
        frames[i].pagina = -1;  // -1 indica marco libre
        frames[i].usado = 0;
    }

    int puntero = 0;   // puntero del reloj
    int refs = 0;      // total de referencias
    int fallos = 0;    // total de fallos de página

    char linea[64];

    // Leer una dirección virtual por línea (decimal o 0xHEX)
    while (fgets(linea, sizeof(linea), f)) {
        // strtoull interpreta automáticamente 0x como hex
        uint64_t DV = strtoull(linea, NULL, 0);
        refs++;

        // Descomponer DV en página virtual (nvp) y offset
        uint64_t offset = DV & mask;
        uint64_t nvp = DV >> b;

        int hit = 0;
        int marco = -1;

        // 1) Buscar HIT
        for (int i = 0; i < N; i++) {
            if (frames[i].pagina == (int)nvp) {
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
                if (frames[i].pagina == -1) {
                    frames[i].pagina = (int)nvp;
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

                // Reemplazar víctima en frames[puntero]
                marco = puntero;
                frames[marco].pagina = (int)nvp;
                frames[marco].usado = 1;

                // Avanzar puntero para la próxima víctima
                puntero = (puntero + 1) % N;
            }
        }

        // 3) Calcular dirección física DF = (marco << b) | offset
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
    printf("Fallos de página: %d\n", fallos);
    printf("Tasa de fallos: %.4f\n", tasa);

    free(frames);
    return 0;
}
