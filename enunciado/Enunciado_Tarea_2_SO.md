# Tarea 2

## Sistemas Operativos

## Segundo Semestre 2025, Prof. Cecilia Hernández y Juan Felipe González

### 11 de noviembre de 2025

Fecha Inicio: Martes 11 de noviembre, 2025.
Fecha Entrega: Lunes 1 de diciembre, 2025 (23:59 hrs).
Metodología: Trabajo en grupo: Integrado con 3 o 4 estudiantes. No se aceptarán
trabajos individuales.

## Contenidos

Esta tarea tiene dos partes, una relacionada con las primitivas de sincronización y otra con
el manejo de memoria virtual. Cada parte tiene sus propios objetivos, descripción y actividades
a realizar.

## Parte I. Sincronización con Barrera reutilizable

1. Objetivos

```
Diseñar e implementar una barrera reutilizable que coordine múltiples hebras concu-
rrentes.
Aplicar la conceptualización de monitor usandopthread_mutex_typthread_cond_t
de la biblioteca pthread.
Razonar sobre condiciones de carrera, exclusión mutua, espera condicional usando
variables de condición y conceptualización de monitor.
Construir una aplicación de verificación que demuestre la correctitud de su implemen-
tación de barrera.
```
2. Definición
    Una barrera es primitiva que sincroniza aNhebras concurrentes en puntos de encuentro:
    ninguna hebra puede continuar hasta que todas hayan llegado. Una barrera reutilizable es
    una barrera que permite sincronizar las hebras en distintos puntos de encuentro. Para ello,
    la barrera debe resetease automáticamente cuando todas las hebras llegan a un punto de
    encuentro y pasan a una siguiente etapa.

### Actividad 1: Implementación de la barrera (Monitor) (2.0 puntos) KPIs 1.2 y

### 1.

```
Implemente una barrera reutilizable como unmonitor:
```
1. Estado compartido mínimo recomendado:


```
int count;número de hebras que han llegado a la barrera en la etapa actual.
int N;número total de hebras que deben esperar (parámetro de inicialización).
int etapa;identificador de etapa donde operanNhebras.
```
2. Sincronización:

```
Unpthread_mutex_tpara la exclusión mutua del estado interno.
Unpthread_cond_tpara que los hebras esperen hasta que lleguen todas al punto de
espera.
```
3. Operaciones

```
La barrera debe poder crearse e inicializarse para sincronizarNhebras, destruirse y
soportar su operación fundamental que es elwait(), la que bloquea la hebra si es que
no es la última de lasN.
```
4. Restricciones

```
Usarúnicamentepthread_mutex_typthread_cond_tcomo primitivas de sincroni-
zación (no se permite el uso de semáforos ni la barrera definida en pthreads, es decir
pthread_barrier_t).
La implementación debe ser reutilizable, es decir se pueda usar el mismo objeto en
múltiples etapas.
El código debe compilar en Linux congccy la opción-pthread.
```
5. Pistas de diseño

```
Capturar laetapaactual en una variable local y esperar mientras no cambie.
La última en llegar debe: (i) incrementaretapa, (ii) resetearcount, (iii)broadcast
para despertar a todas.
Usar el patrón:lock→modificar estado / decidir→wait/broadcast→unlock.
```
### Actividad 2: Aplicación de verificación (1.0 puntos) KPIs 1.2 y 1.

```
Implemente un programamain.cque verifique el comportamiento de su barrera. Debe:
```
1. CrearNhebras (parámetro de línea de comandos, por defectoN= 5).
2. EjecutarEetapas(parámetro, por defectoE= 4). En cada etapa, cada hebra:

```
a) Realiza trabajo simulado (usleepcon±aleatorio).
b) Imprime antes de esperar:[tid] esperando en etapae.
c) Llama await().
d) Imprime después de pasar:[tid] paso barrera en etapae.
```
3. Verificación:

```
Para cada etapae, todas las hebras deben imprimir el mensaje “pasó barrera en etapa
e)” después de quetodashayan impreso el mensaje “esperando en etapae”.
La secuencia de impresiones puede intercalarse entre las hebras, pero ninguna hebra
debe pasar la barrera de la etapaea lae+ 1antes de que todas hayan llegado al punto
de encuentro.
```

## Parte II. Simulador simple de Memoria Virtual

1. Objetivos

```
Implementar un simulador secuencial de memoria virtual que procese una traza de
páginas virtuales.
Soportar el algoritmo de reemplazo Reloj.
Medir y reportar el número de fallos de página y la tasa de fallos para distintas
parámetros de configuración.
```
2. Descripción
    En esta parte debe implementar un simulador,simcon paginación simple y soporte de
    algoritmo de reemplazo Reloj con un solo puntero.

### Actividad 1: Simulador de Traducción de Direcciones

Implemente un simulador,sim, que lea una traza dedirecciones virtuales(una por línea, en
decimal o0xHEX), realice la traducción a direcciones físicas usandopaginación simple, y reporte
el número de fallos de páginay ladirección físicaresultante. Mantenga un conjunto demarcos
físicos de tamaño fijo y use una política simple de asignación: si hay marcos libres, asignar; si
no, reemplazar usando el algoritmo delReloj.

1. Definición: Traducción en hexadecimal
    Sea una dirección virtualDVen hexadecimal, con un tamaño de marco de páginaPAGE_SIZE =
    2 bbytes. Sea ademásMASK = 2b− 1 el valor que permite extraer los bits de desplaza-
    miento (offset) dentro de la página. Recuerde que en notación binaria, la dirección virtual
    se divide en componente nvp y offset:
       Descomposición de la dirección virtual:

```
offset = VA & MASK,
npv = DV≫b.
```
```
donde&denota la operaciónANDbit a bit y≫bel desplazamiento a la derecha de
bbits.
```
```
Cálculo de la dirección física:
Si la tabla de páginas asocia la página virtualnvpa un marco físicomarco, entonces
la dirección física (DF) se obtiene como:
```
```
DF = (marco≪b)|offset,
```
```
donde≪brepresenta el desplazamiento a la izquierda (equivalente a multiplicar por
2 b), y|la operaciónORbit a bit.
```
2. Reglas de traducción

```
DV compuesta por npv y offset usando PAGE_SIZE.
Si npv está en laTabla de páginashay unHIT: luego, se obtiene marco y computa
DF.
Si no está hay unFALLO: luego, si hay marco libre asignarlo a pv; en otro caso,
expulsar víctima según algoritmo delReloj. Actualizar la tabla de páginas (invalidar
npv expulsada, mapear npv con fallo a marco).
```

3. Parámetros del simulador

```
./sim Nmarcos tamañomarco [--verbose] traza.txt
```
```
Donde Nmarcos corresponde al número de margos disponibles, tamaño
del marco en bytes y verbose es opcional e indica la secuencia paso a paso en
el proceso de traducción. En caso de no realizarse solo reportar el número
de páginas.
```
```
# Ejemplo:
./sim 8 4096 traza.txt
```
4. Salida de simulación
    Totales: Referencias, Fallos de página, y Tasa de fallos.
    Con–verbose: por cada DV, imprimir: DV, nvp, offset,HIT/FALLO, marco usado, DF
    calculada.
5. Datos de prueba
    Genere entre 5 y 10 referencias y verifique sus resultados.
    Use los dos archivos de referencias en traces.zip y realice un análisis experimental para
    ver como se comporta la tasa de fallos variando el número de marcos disponibles.
    Considere los siguientes rangos de marcos de páginas(8, 16 ,32)y tamaño de marco
    de 8 para trace1.txt y 4096 para trace2.txt.

## Entregables

```
Sus implementaciones deben ser propocionadas en un repositorio por grupo.
```
```
Su repositorio debe contener un Readme donde se proporcione información de compilación
y modo de uso.
```
```
Un informe que describa los principales componentes de su implementación y su evaluación
de resultados.
```

