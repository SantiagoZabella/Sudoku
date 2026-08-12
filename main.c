# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include "sudoku.h"

void test_sudoku(int n)
{
    printf("\n\nTEST %d x %d\n", n, n);

    Tablero tab = crear_sudoku_unica_solucion(n);
    if (!tab)
    {
        printf("ERROR: no se pudo generar sudoku\n");
        return;
    }

    int **solucion = crear_matriz(n);
    tab->cantSoluciones = 0;
    int cantSoluciones = resolver(tab, solucion);

    printf("Soluciones: %d\n", cantSoluciones);
    printf("Tablero generado:\n");
    imprimir_matriz(tab->matriz, n);
    
    printf("Solucion encontrada:\n");
    imprimir_matriz(solucion, n);
    
    liberar_matriz(solucion, n);
    liberar_tablero(tab);
}


int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Los datos a ingresar son: <programa> <cnombre de archivo>\n");
        return 1;
    }
    
    srand(time(NULL));    

    char* nombreArchivo = argv[1];

    Tablero tablero;

    tablero = crear_tablero_desde_archivo(nombreArchivo);
    if(!tablero){
        printf("Fallo la creacion del tablero!\n");
        return 1;
    }

    int **matrizAux = crear_matriz(tablero->tamaño);

    int tableroResuelto = resolver(tablero, matrizAux);

    if(tableroResuelto == 1){
        printf("La unica solucion del sudoku es:\n");
        imprimir_matriz(matrizAux, tablero->tamaño);
    }
    else if (tableroResuelto == 2){
        printf("El sudoku tiene mas de una solucion, una de ellas es:\n");
        imprimir_matriz(tablero->matriz, tablero->tamaño);
    }
    
    else{
        printf("El sudoku no tiene solucion!\n");
        imprimir_matriz(tablero->matriz, tablero->tamaño);
    }
    
    liberar_matriz(matrizAux, tablero->tamaño);


    liberar_tablero(tablero);
    
    test_sudoku(9);

    test_sudoku(9);

    test_sudoku(9);

    return 0;
}