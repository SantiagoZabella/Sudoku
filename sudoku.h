# ifndef __SUDOKU_H__
# define __SUDOKU_H__
# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

/**
 * La estructura tablero esta formada por:
 *      * Una matriz nxn (arreglo de arreglos de enteros)
 *      * El tamaño del sudoku 
 *      * La cantidad de casillas vacias (con 0s)
 *      * El tamaño de los subcuadrados que hay que validar
 *      * La cantidad de soluciones que posee el sudoku
 */
typedef struct {
    int **matriz;
    int tamaño;
    int casillasVacias;
    int tamañoSubcuadrado;
    int cantSoluciones;
} Tablero_;

typedef Tablero_ *Tablero;

/**
 * Crea una estructura tablero vacia y la devuelve, o devuelve NULL de no haber podido
 */
Tablero crear_tablero_vacio(int tamaño);

/**
 * Esta funcion reserva espacio para una matriz nxn
 */
int **crear_matriz(int n);

/**
 * Dado un archivo por argumento y un tablero, esta funcion rellena las 
 * casillas de la matriz del tablero y actualiza la cantidad de casillas vacias
 */
void cargar_tablero(FILE* archivo, Tablero t);

/**
 * Dado el nombre de un archivo, crea un tablero y lo inicializa con los datos 
 * del archivo
 */
Tablero crear_tablero_desde_archivo(char *archivoSudoku);


/**
 * La funcion busca la casilla vacia con menos opciones para instertar y devuelve 1 si la encuentra
 * o 0 si el sudoku se completo
 */
int mejor_casilla_vacia(Tablero tab, int *fila, int *columna);

/**
 * Retorna 1 si el numero pasado es valido para colocar en la fila o 0
 * en caso contrario
 */
int validar_fila(Tablero tab, int fila, int num);

/**
 * Retorna 1 si el numero pasado es valido para colocar en la columna o 0
 * en caso contrario
 */
int validar_columna(Tablero tab, int columna, int num);

/**
 * Retorna 1 si el numero pasado es valido para colocar en el subcuadrado o 0
 * en caso contrario
 */
int validar_subcuadrado(Tablero tab, int fila, int columna, int num);

/**
 * Retorna 1 si el numero pasado es valido para colocar en la 
 * posicion (fila, columna)o 0 en caso contrario
 */
int es_casilla_valida(Tablero tab,int fila, int columna, int num);

/**
 * Realiza una copia fisica de la matriz de origen en la matriz de destino
 */
void copiar_matriz(int **origen, int **destino, int tamaño);

/**
 * Retorna 1 si hay una unica solucion y la imprime, 0 si no tiene solucion e
 * imrpime el tablero sin completar o 2 si encuentra mas de una solucion e
 * imprime la segunda solucion qu encunetra
 */
int resolver(Tablero tab, int **matrizSolucion);

/**
 * Imprime el tablero ingresado
 */
void imprimir_matriz(int ** matriz, int tamaño);

/**
 * Libera el espacio reservado para el tablero y su matriz
 */
void liberar_tablero(Tablero tab);

/**
 * Libera el espacio reservado para una matriz
 */
void liberar_matriz(int **matriz, int tamaño);

/**
 * Devuelve una posicion aleatoria de 0 a tamaño - 1
 */
int pos_aleatoria(int tamaño);

/**
 * Desordena el array pasado de n elementos
 */
void desordena_array(int *arr, int n);

/**
 * Completa la matriz de un tablero vacio creando una lista de tamaño tab->tamaño
 * y desordenandola para ir llenando la matriz con valores random; funciona de manera
 * muy similar al algoritmo de la funcion resolver, exceptuando que los numeros que
 * se prueban no estan en orden y que al encontrar una solucion se detiene
 */
int completa_tablero_vacio(Tablero tab);

/**
 * Genera un tablero vacio para luego completarlo con una llamada a la funcion
 * completa_tablero_vacio
 */
Tablero genera_tablero_resuelto(int tamaño);

/**
 * Remueve casillas aleatorias de la matriz del tablero chequeando en cada eliminacion
 * que el sudoku tenga una unica solucion, reutiliza la funcion resolver para esto,
 * y ademas lleva una copia de la matriz en caso de que al eliminar una casilla el
 * sudoku tenga 2 soluciones, copia nuevamente la matriz donde habia 1 solucion
 */
void remover_casillas(Tablero tab, int **matrizAux);

/**
 * Crea un tablero a partir de un tamaño dado, lo llena y luego le quita la mayor
 * cantidad de casillas posibles, siempre manteniendo el criterio de que tenga una
 * unica solucion
 */
Tablero crear_sudoku_unica_solucion(int tamaño);


#endif /*__SUDOKU_H__*/
