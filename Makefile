
CC = gcc
CFLAGS = -Wall -O2
PTHREAD = -pthread

BARRERA_SRC = src/barrera/barrera.c src/barrera/main.c
BARRERA_BIN = src/barrera/btest

SIM_SRC = src/reloj/sim.c
SIM_BIN = src/reloj/sim

.PHONY: all barrera sim clean run-examples

all: barrera sim

barrera:
	$(CC) $(CFLAGS) $(BARRERA_SRC) -o $(BARRERA_BIN) $(PTHREAD)

sim:
	$(CC) $(CFLAGS) $(SIM_SRC) -o $(SIM_BIN)

clean:
	rm -f $(BARRERA_BIN) $(SIM_BIN)

run-barrera: barrera
	@echo "Ejecutando verificacion de barrera (N=5, E=4)..."
	@./$(BARRERA_BIN) 5 4

run-sim: sim
	@echo "Ejecutando simulador con trace1 (8 marcos, 4096 bytes)..."
	@./$(SIM_BIN) 8 4096 src/reloj/testing/trace1.txt
	@echo ""
	@echo "Ejecutando simulador con trace2 (8 marcos, 4096 bytes)..."
	@./$(SIM_BIN) 8 4096 src/reloj/testing/trace2.txt

run-examples: run-barrera run-sim

