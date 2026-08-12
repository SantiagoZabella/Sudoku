# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "sudoku.h"
# include <time.h>

Tablero crear_tablero_vacio(int tamaño){
    int n = sqrt(tamaño);
    if (tamaño != n * n) return NULL;                                   // Chequeo que la raiz el tamaño sea un cuadrado perfecto

    Tablero tab = malloc(sizeof(Tablero_));
    if(!tab) return NULL;

    tab->cantSoluciones = 0;
    tab->casillasVacias = tamaño*tamaño;
    tab->matriz = crear_matriz(tamaño);
    tab->tamaño = tamaño;
    tab->tamañoSubcuadrado = n;

    if(!tab->matriz){ 
        free(tab);
        return NULL;
    }
    

    return tab;
}

int **crear_matriz(int n){
    int **matriz = malloc(n * sizeof(int*));
    if(!matriz) return NULL;
    for(int i = 0; i < n; i++){
        matriz[i] = calloc(n, sizeof(int));                             // Inicializa las casillas con 0
        if(!matriz[i]){
            for(int j = 0; j < i; j++){
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

void cargar_tablero(FILE* archivo, Tablero tab){
    
    tab->casillasVacias = 0;

    char c;

    for(int i = 0; i < tab->tamaño; i++){
        for(int j = 0; j < tab->tamaño; j++){
            c = fgetc(archivo);                                         // Leo de a 1 caracter
            
            if(c >= '0' && c <= '9') 
                tab->matriz[i][j] = c - '0';                            // Chequeo que sea un numero entre 0 y 9 con el codigo ASCII
            
            if(tab->matriz[i][j] == 0) tab->casillasVacias++;
        }
    }
}

Tablero crear_tablero_desde_archivo(char *archivoSudoku){
    FILE* archivo = fopen(archivoSudoku, "r");
    if (!archivo){
            printf("Error al abrir archivo\n");
            return NULL;
        }
    
    int cantElementos = 0;

    char c;

    while((c = fgetc(archivo)) !=EOF){
        if(c >= '0' && c <= '9') cantElementos++;                       // Incremento la cantidad de caracteres leidos si es un decimal
    }

    rewind(archivo);

    int tamaño = sqrt(cantElementos);
    if(tamaño * tamaño != cantElementos){                               // Chequeo que cantidad de elementos sea un cuadrado pefecto
        fclose(archivo);
        return NULL;
    }
    
    Tablero tab = crear_tablero_vacio(tamaño);
    if(!tab){ 
        fclose(archivo);
        return NULL;
    }
    cargar_tablero(archivo, tab);

    fclose(archivo);

    return tab;
}

int mejor_casilla_vacia(Tablero tab, int *fila, int *columna){
    int minOpciones = tab->tamaño + 1;
    int mejor_casilla_fila = -1;
    int mejor_casilla_columna = -1;

    for(int i = 0; i < tab->tamaño; i++){
        for(int j = 0; j < tab->tamaño; j++){
            if(tab->matriz[i][j] == 0){                                 // Encuentro una casilla vacia

                int opciones = 0;
                for(int k = 1; k <= tab->tamaño; k++){                  // Verifico cuantos numeros son validos de insertar
                    if(es_casilla_valida(tab, i, j, k))
                        opciones++;

                }

                if(opciones < minOpciones){                             // Me quedo con la fila y la columna que menos opciones tenga para ingresar
                    minOpciones = opciones;
                    mejor_casilla_fila = i;
                    mejor_casilla_columna = j;
                }
            }
        }
    }

    if(mejor_casilla_fila == -1) return 0;

    *fila = mejor_casilla_fila;
    *columna = mejor_casilla_columna;
    return 1;
}


int validar_fila(Tablero tab, int fila, int num){
    for (int i = 0; i < tab->tamaño; i++){
        if(tab->matriz[fila][i] == num) return 0;
    }
    return 1;
}

int validar_columna(Tablero tab, int columna, int num){
    for (int i = 0; i < tab->tamaño; i++){
        if(tab->matriz[i][columna] == num) return 0;
    }
    return 1;
}

int validar_subcuadrado(Tablero tab, int fila, int columna, int num){
    int n = tab->tamañoSubcuadrado;
    int filaInicio = (fila / n) * n;
    int columnaInicio = (columna / n) * n;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(tab->matriz[i + filaInicio][j + columnaInicio] == num) return 0;
        }
    }

    return 1;
}


int es_casilla_valida(Tablero tab,int fila, int columna, int num){
    return (validar_fila(tab, fila, num) && 
           validar_columna(tab, columna, num) && 
           validar_subcuadrado(tab, fila, columna, num));
}

void copiar_matriz(int **origen, int **destino, int tamaño){
    for(int i = 0; i < tamaño; i++){
        for(int j = 0; j < tamaño; j++){
            destino[i][j] = origen[i][j];
        }
    }
}

int resolver(Tablero tab, int **matrizSolucion){ 
    int fila, columna; 

    //casos base    
    if(tab->cantSoluciones >= 2) return tab->cantSoluciones;                // Si encuentra mas de una solucion corta la recursion 
    
    if(!mejor_casilla_vacia(tab, &fila, &columna)){ 
        tab->cantSoluciones++;                                              // Si se completa el sudoku, continua buscando mas soluciones 
        
        if(tab->cantSoluciones == 1) 
            copiar_matriz(tab->matriz, matrizSolucion, tab->tamaño); 
        
        return tab->cantSoluciones; 
    }
     
    // paso recursivo 
    for(int i = 1; i <= tab->tamaño; i++){ 
        if(es_casilla_valida(tab, fila, columna, i)){ 
            tab->matriz[fila][columna] = i;                                 // Rellena la casilla el mejor candidato 
            tab->casillasVacias--; 
            
            if(resolver(tab,matrizSolucion) >= 2) 
                return tab->cantSoluciones;                                 // Sigue rellenando hasta que un numero no se pueda colocar, o hasta llenar el sudoku 
                                                                            // Si un numero no se puede colocar, vuelve sobre la recursion y prueba con el siguiente numero
            tab->matriz[fila][columna] = 0;                                  
            tab->casillasVacias++; 
        } 
    } 
    return tab->cantSoluciones; 
}


void imprimir_matriz(int ** matriz, int tamaño){
    for(int i = 0; i < tamaño; i++){
        for(int j = 0; j < tamaño; j++){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void liberar_tablero(Tablero tab){
    for(int i = 0; i< tab->tamaño; i++){
        free(tab->matriz[i]);
    }
    free(tab->matriz);
    free(tab);
}

void liberar_matriz(int **matriz, int tamaño){
    for(int i = 0; i < tamaño; i++){
        free(matriz[i]);
    }
    free(matriz);
}

int pos_aleatoria(int tamaño){
    return rand() % tamaño;
}

void desordena_array(int *arr, int n){
    for (int i = n-1; i > 0; i--){
        int j = pos_aleatoria(i+1);
        int aux = arr[i];
        arr[i] = arr[j];
        arr[j] = aux;
    }
}

int completa_tablero_vacio(Tablero tab){

    int fila, columna;
    // caso base         
    if(!mejor_casilla_vacia(tab, &fila, &columna)){                             // Si ya no quedan casillas vacias por probar, se lleno el sudoku             
        tab->cantSoluciones++; 
        return 1;                                                                      
    }

    int *candidatos = malloc(sizeof(int) * tab->tamaño);
    for(int i = 0; i < tab->tamaño; i++) candidatos[i] = i + 1;  
    desordena_array(candidatos, tab->tamaño);                                   // Desordeno el array en cada llamada

    // paso recursivo
    for(int i = 0; i < tab->tamaño; i++){
        int candidato = candidatos[i];
        if(es_casilla_valida(tab, fila, columna, candidato)){
            tab->matriz[fila][columna] = candidato;                         
            tab->casillasVacias--;
            if(completa_tablero_vacio(tab)){                                    // Llamo a recursion 
                free(candidatos);
                return 1;
            }      
            tab->matriz[fila][columna] = 0;                                     // Si no puedo completar la casilla, la vuelvo a poner en 0 y sigo probando
            tab->casillasVacias++;
        }
    }
    free(candidatos);
    return 0;
}

Tablero genera_tablero_resuelto(int tamaño){
    Tablero tab = crear_tablero_vacio(tamaño);
    if(!tab) return NULL;
    
    if(!completa_tablero_vacio(tab)){
        liberar_tablero(tab);
        return NULL;
    }
    return tab;
}


void remover_casillas(Tablero tab, int **matrizAux){
    int cantElementos = tab->tamaño * tab->tamaño;
    int posiciones[cantElementos];                                              // Crea y desordena un array para ir removiendo posiciones aleatorias

    for(int i = 0; i < cantElementos; i++){
        posiciones[i] = i;
    }
    desordena_array(posiciones, cantElementos); 

    int **copia = crear_matriz(tab->tamaño);                                    // Copia de seguridad en caso de que al sacar un elemento, el sudoku
                                                                                // tenga mas de una solucion
    for(int i = 0; i < cantElementos; i++){         
        int pos = posiciones[i];                                                // Localiza a que fila pertenece la posicion
        int fila = pos / tab->tamaño;                                           // Localiza a que columna pertenece la posicion
        int columna = pos % tab->tamaño;            

        copiar_matriz(tab->matriz, copia, tab->tamaño);                         // Hace backup de la matriz
        int casillasVacias = tab->casillasVacias;           

        tab->matriz[fila][columna] = 0;         
        tab->casillasVacias++;          
        tab->cantSoluciones = 0;            
        int resultado = resolver(tab, matrizAux);           

        if(resultado != 1){                                                     // Si al eliminar un elemento el sudoku tiene mas de 1 solucion
            copiar_matriz(copia, tab->matriz, tab->tamaño);                     // utiliza el backup de la matriz para volver a cuando tenia una unica solucion
            tab->casillasVacias = casillasVacias;                               // y quedarse con esa
        }
    }

    liberar_matriz(copia, tab->tamaño);
}

Tablero crear_sudoku_unica_solucion(int tamaño){
    
    Tablero tab = genera_tablero_resuelto(tamaño);                              // Creo un tablero aleatorio completo
    if(!tab) return NULL;
    
    int **matrizAux = crear_matriz(tab->tamaño);                                // Llevo una copia de seguridad para ir quitando casillas con tranquilidad
    if(!matrizAux){
        liberar_tablero(tab);
        return NULL;
    }
    
    remover_casillas(tab, matrizAux);                                           // Remuevo casillas hasta que el sudoku tenga 2 soluciones, luego
                                                                                // gracias a la copia de seguridad, devuelvo la ultima matriz con solucion unica
    liberar_matriz(matrizAux, tab->tamaño);

    return tab;
}

