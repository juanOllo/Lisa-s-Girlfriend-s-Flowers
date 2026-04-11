#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <time.h>
// #include <pthread.h>

// #include <assert.h>
#include <unistd.h>

// ESTOS AUX SON PARA QUE FUNCIONE getch() y cls()
//#include <conio.h>
#include "auxLibsWin.h"
// #include "auxLibsUbu.h"

#include <pthread.h>
#include <stdbool.h>

#define maxV 17
#define maxH 62
#define SIZE 200

int osMultiplicadorDeVelocidad = velocity;

typedef struct {
	int x;
	int y;
} coor;

typedef struct {
	int hp;                 // puntos de hp de ndl
	int cantFlores;         // cantidad de flores en mano
	int misionesCumplidas;  // cantidad de flores entregadas
	int primeraVez;         // aux q voy a usar para las cinematicas creo
	int lucides[4];         // guarda las veces q gastaste todos los dialogos a cada vecino
	coor ubi;               // ubicacion actual de ndl
	int calleLoc;           // en que seccion del juego se encuentra ndl
    int colorL;
} Player;


// No es necesario pero las defino aca para encontrarlas mas rapido
void actualizarHP(int hp);  
void actualizarMenuVecino(int vecinoFocusOption);
void cambiarCalleAnim(char escena[maxV][maxH], char escena2[maxV][maxH], char direccion);
void cargarEscenas(char escenaVisual[maxV][maxH], char escenaLimites[maxV][maxH], int l);
void cargarEscenasConAlternativa(char escenaVisual[maxV][maxH], char escenaVisualAlternativa[maxV][maxH], char escenaLimites[maxV][maxH], int l);
void guardarPartida(Player *ndl, int typeSave);
void leerEscuchar(int cont);
coor movimientoConInput(char input, coor actualUbi, char movimientoLimit[maxV][maxH], int casaOCalle);
void printNdl(int colorL);
int proximo(coor u, coor e);
int proximoAbeja(coor ndl, coor abeja);
coor randomUbi(coor e, char limit[maxV][maxH]);
void ubicarPivote();

// Funciones solo para debuguear
void debugString(char errorMsj[100]);
void debugStringInt(char msj[100], int a);
void debugChar(char a);
void debugEscena(char escena[maxV][maxH]);
void debugNdlData(Player *ndl);














void actualizarHP(int hp){
    
    // printf("\n\n\t                                      ");
    printf("\n\n\t\t                                   ");
    switch (hp){
        case 80:
            printf(" ENERGIA: [////////] \n");
            break;
        case 70:
            printf(" ENERGIA: [/////// ] \n");
            break;
        case 60:
            printf(" ENERGIA: [//////  ] \n");
            break;
        case 50:
            printf(" ENERGIA: [/////   ] \n");
            break;
        case 40:
            printf(" ENERGIA: [////    ] \n");
            break;
        case 30:
            printf(" ENERGIA: [///     ] \n");
            break;
        case 20:
            printf(" ENERGIA: [//      ] \n");
            break;
        case 10:
            printf(" ENERGIA: [/       ] \n");
            break;
        case 0:
            printf(" ENERGIA: [        ] \n");
            break;
        default:
            break;
    }

    printf("\e[%iA", 3);
}




void actualizarMenuVecino(int vecinoFocusOption){
    switch (vecinoFocusOption){
        case 1:
            printf("\t\t\t\t\t _          .n.  _                |     |    \n");
            printf("\t\t\t\t\t|          C O D  |               |  |  |    \n");
            printf("\t\t\t\t\t| Dar Flor  *Y*   |       Volver  |     |    \n");
            printf("\t\t\t\t\t|_           |   _|               |  |  |    \n");
            break;

        case 2:
            printf("\t\t\t\t\t            .n.          _        |     | _   \n");
			printf("\t\t\t\t\t           C O D        |         |  |  |  |  \n");
			printf("\t\t\t\t\t  Dar Flor  *Y*         | Volver  |     |  |  \n");
			printf("\t\t\t\t\t             |          |_        |  |  | _|  \n");
            break;
        
        default:
            break;
    }
    printf("\e[%iA", 4);
}



                    //                                                  'i' = izquierda
                    // Escena actual            Escena a la q vas       'd' = derecha
void cambiarCalleAnim(char escena[maxV][maxH], char escena2[maxV][maxH], char direccion){
	printf("\e[%iA", 15);

    int k = 1;

    switch (direccion){
        // DERECHA
        case 'd':
            while (k < maxH-1){
                
                for(int i = maxV-1; i > 1; i--){
                    printf("                              ");
                    for(int j = k; j < maxH-1; j++){
                        printf("%c", escena[maxV-i][j]);
                    }

                    for (int d = 1; d < (k+1); d++){
                        printf("%c", escena2[maxV-i][d]);
                    }

                    // El ' ' en el printf es para borrar la flor q se buguea a la derecha de la escena de la calle.
                    printf(" \n");
                }
                printf("\e[%iA", 15);

                k++;
            }
            break;

        // IZQUIERDA
        case 'i':
            while (k < maxH-2){
                
                for(int i = maxV-1; i > 1; i--){
                    printf("                              ");
                    // Escena a la q voy
                    for (int d = ((maxH-2)-k); d < (maxH-1); d++){
                        printf("%c", escena2[maxV-i][d]);
                    }

                    // Escena en la q estoy
                    for(int j = 1; j < (maxH-2-k); j++){
                        printf("%c", escena[maxV-i][j]);
                    }

                    // El ' ' en el printf es para borrar la flor q se buguea a la derecha de la escena de la calle.
                    printf(" \n");
                }
                printf("\e[%iA", 15);

                k++;
            }
            break;
        
        default:
            break;
    }

    
    
}




// CARGA 2 MATRIZES CON ESCENAS DEL DOCUMENTO escenas.txt 
//  COMENZANDO EN LA LINEA l
//  CARGA LAS ESCENA QUE SE MUESTRA EN PANTALLA Y LA QUE MARCA LOS LIMITES AL MOVIMIENTO
void cargarEscenas(char escenaVisual[maxV][maxH], char escenaLimites[maxV][maxH], int l){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    char linea[maxH+2];
    
    f = fopen("escenas.txt", "r");
    
    while(i < l-1){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        i++;
    }
    
    if(f != NULL){

        // Carga la escena que se va a mostrar en pantalla.
        for (i = 0; i < maxV; i++){
            for(j = 0; j < maxH; j++){
                fscanf(f, "%c", &escenaVisual[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que va a marcar los limites.
        for (i = 0; i <= maxV; i++){
            for(j = 0; j < maxH; j++){
                fscanf(f, "%c", &escenaLimites[i][j]);
            }
            fscanf(f, "%c", &aux);
        }
		
    } else {
        printf("error");
    }

	fclose(f);
}




// CARGA 3 MATRIZES CON ESCENAS DEL DOCUMENTO escenas.txt 
//  COMENZANDO EN LA LINEA l
//  CARGA LAS 2 ESCENAS QUE SE ALTERNAN EN PANTALLA Y LA QUE MARCA LOS LIMITES
void cargarEscenasConAlternativa(char escenaVisual[maxV][maxH], char escenaVisualAlternativa[maxV][maxH], char escenaLimites[maxV][maxH], int l){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    char linea[maxH+2];
    
    f = fopen("escenas.txt", "r");
    
    while(i < l-1){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        i++;
    }
    
    if(f != NULL){

        // Carga la escena que se va a mostrar en pantalla.
        for (i = 0; i < maxV; i++){
            for(j = 0; j < maxH; j++){
                fscanf(f, "%c", &escenaVisual[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que se va a alternar visualmente con la primera.
        for (i = 0; i < maxV; i++){
            for(j = 0; j < maxH; j++){
                fscanf(f, "%c", &escenaVisualAlternativa[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que va a marcar los limites.
        for (i = 0; i <= maxV; i++){
            for(j = 0; j < maxH; j++){
                fscanf(f, "%c", &escenaLimites[i][j]);
            }
            fscanf(f, "%c", &aux);
        }
		
    } else {
        printf("error");
    }

	fclose(f);
}




// Guarda TODOS LOS DATOS DE NDL EN save.txt
// typeSabe = 0 para guardado automatico
//          = 1 para guardado manual
void guardarPartida(Player *ndl, int typeSabe){

    char toSaveArray[18];

    // Asi guardo solo el color de ndl
    if (typeSabe == 2){
        FILE *save;
        save = fopen("save.txt", "r+");

        // Nueve el puntero dos caracteres antes del final
        fseek(save, -2, SEEK_END);
        fprintf(save, "%d", ndl->colorL);
        fclose(save);
        return;
    }

    sprintf(&toSaveArray[0], "%d", ndl->cantFlores);
    
    sprintf(&toSaveArray[1], "%d", ndl->misionesCumplidas);
    
    sprintf(&toSaveArray[2], "%d", ndl->primeraVez);
    
    sprintf(&toSaveArray[3], "%d", ndl->lucides[0]);
    sprintf(&toSaveArray[4], "%d", ndl->lucides[1]);
    sprintf(&toSaveArray[5], "%d", ndl->lucides[2]);
    sprintf(&toSaveArray[6], "%d", ndl->lucides[3]);

    switch (ndl->hp){
        // case 100:
        //     sprintf(&toSaveArray[7], "%d", ndl->hp);
        //     break;
        case 0:
            // sprintf(&toSaveArray[7], "%d", 0);
            // sprintf(&toSaveArray[8], "%d", 0);
            // sprintf(&toSaveArray[9], "%d", ndl->hp);
            sprintf(&toSaveArray[7], "000");
            break;
        
        // case 25:
        // case 50:
        // case 75:
        // break;
        
        default:
            sprintf(&toSaveArray[7], "0");
            sprintf(&toSaveArray[8], "%d", ndl->hp);
            break;
    }

    if(ndl->ubi.x > 9){
        sprintf(&toSaveArray[10], "%d", ndl->ubi.x);
    } else {
        sprintf(&toSaveArray[10], "0");
        sprintf(&toSaveArray[11], "%d", ndl->ubi.x);
    }
    
    if(ndl->ubi.y > 9){
        sprintf(&toSaveArray[12], "%d", ndl->ubi.y);
    } else {
        sprintf(&toSaveArray[12], "0");
        sprintf(&toSaveArray[13], "%d", ndl->ubi.y);
    }

    sprintf(&toSaveArray[14], "%d", ndl->calleLoc);

    sprintf(&toSaveArray[15], "%d", ndl->colorL);

    // Guardo la informacion de toSaveArray en save.txt
    FILE *save;
	save = fopen("save.txt", "w");
	fprintf(save, toSaveArray);
	fclose(save);

    // La barra de carga es falsa jsdjj
    if (typeSabe == 1){
        printf("\e[%iA", 1);
        printf("\n\t---+ [GUARDANDO PARTIDA                                                                           ] +");
		printf("\e[%iA\r", 1);
        sleep_ms(500);
        printf("\n\t\t\t\t##############################");
        sleep_ms(500);
        printf("#######");
        sleep_ms(200);
        printf("############");
        sleep_ms(500);
        printf("#######################");
        sleep_ms(300);
        printf("##");
        sleep_ms(500);
    }
    
}




//  PRINTEA DIALOGO DE PERSONAJE
void leerEscuchar(int cont){

    FILE *f;

	char frase[SIZE];
	char dialogo[SIZE];
	int i = 1;
	f= fopen("escuchar.txt","r");
	//printf("\nEntro a escuchar.\n");
	
	if (f!=NULL){
		while(!feof(f)){
			fgets(frase, sizeof(frase), f);	
			while (i <= cont){
			//switch(i){
			//	case 1 :{
					//printf("\nFrase01: %s\n", frase);
					strcpy( dialogo, frase);
					i++;
					break;
			//	}
			//}
			}
		}
	}//while(i != 0);
	printf("\t%s\n", dialogo);
	//printf("\nTermina escuchar.\n");

}




// FUNCION QUE DEVUELVE NUEVA UBI DE NDL CUANDO TE MOVES RESPETANDO LIMITES
//                 input del      ubi actual                                    casa = 10
//                 teclado        de ndl        escLimitCalle/Casa              calle = 0
//                                                                              vecino 1 = 1
//                                                                              vecino 2 = 2
//                                                                              vecino 3 = 3
// 
// WARNING: aunque algunos casos tengan el mismo comportamiento aun no los junto hasta
//  terminar el juego o al menos terminar todos los minijuegos
coor movimientoConInput(char input, coor actualUbi, char movimientoLimit[maxV][maxH], int casaOCalle) {
    coor nuevaUbiAux = actualUbi;

    switch (casaOCalle){
        
        case 10:
            switch (input){
                case 'd':
                case 'D':
                case 77:
                    nuevaUbiAux.x = nuevaUbiAux.x + 3;
                    break;
                case 'a':
                case 'A':
                case 75:
                    nuevaUbiAux.x = nuevaUbiAux.x - 3;
                    break;
                case 'w':
                case 'W':
                case 72:
                    nuevaUbiAux.y = nuevaUbiAux.y + 1;
                    break;
                case 's':
                case 'S':
                case 80:
                    nuevaUbiAux.y = nuevaUbiAux.y - 1;
                    break;
                default:
                    break;
            }
        break;

        case 0:
            switch (input){
                case 'd':
                case 'D':
                case 77:
                    nuevaUbiAux.x = nuevaUbiAux.x + 2;
                    break;
                case 'a':
                case 'A':
                case 75:
                    nuevaUbiAux.x = nuevaUbiAux.x - 2;
                    break;
                case 'w':
                case 'W':
                case 72:
                    nuevaUbiAux.y = nuevaUbiAux.y + 1;
                    break;
                case 's':
                case 'S':
                case 80:
                    nuevaUbiAux.y = nuevaUbiAux.y - 1;
                    break;
                default:
                    break;
            }
        break;

        case 1:
            switch (input){
                case 'd':
                case 'D':
                case 77:
                    nuevaUbiAux.x = nuevaUbiAux.x + 2;
                    break;
                // case 'a':
                // case 'A':
                // case 75:
                //     nuevaUbiAux.x = nuevaUbiAux.x - 2;
                //     break;
                case 'w':
                case 'W':
                case 72:
                    nuevaUbiAux.y = nuevaUbiAux.y + 2;
                    break;
                case 's':
                case 'S':
                case 80:
                    nuevaUbiAux.y = nuevaUbiAux.y - 2;
                    break;
                default:
                    break;
            }
        break;

        case 2:
            switch (input){
                case 'd':
                case 'D':
                case 77:
                    nuevaUbiAux.x = nuevaUbiAux.x + 5;
                    break;
                case 'a':
                case 'A':
                case 75:
                    nuevaUbiAux.x = nuevaUbiAux.x - 5;
                    break;
                case 'w':
                case 'W':
                case 72:
                    nuevaUbiAux.y = nuevaUbiAux.y + 2;
                    break;
                case 's':
                case 'S':
                case 80:
                    nuevaUbiAux.y = nuevaUbiAux.y - 2;
                    break;
                default:
                    break;
            }
        break;

        case 3:
            switch (input){
                case 'd':
                case 'D':
                case 77:
                    nuevaUbiAux.x = nuevaUbiAux.x + 2;
                    break;
                case 'a':
                case 'A':
                case 75:
                    nuevaUbiAux.x = nuevaUbiAux.x - 2;
                    break;
                case 'w':
                case 'W':
                case 72:
                    nuevaUbiAux.y = nuevaUbiAux.y + 1;
                    break;
                case 's':
                case 'S':
                case 80:
                    nuevaUbiAux.y = nuevaUbiAux.y - 1;
                    break;
                default:
                    break;
            }
        break;

        default:
            break;
    }

    if(movimientoLimit[maxV - nuevaUbiAux.y][nuevaUbiAux.x] == '#') {
        return actualUbi;
    } else {
        return nuevaUbiAux;
    }
}




void printNdl(int colorL){
    printf("\033[%dmL\033[0m", colorL);
}




// CHECKEA EL ESTADO ENTRE LAS DOS UBICACIONES 
int proximo(coor u, coor e){

    if(e.x==u.x && e.y==u.y)
        return 2; //Están en el mismo lugar 

    if((e.x+2>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2))
        return 1; //U está en el area de e

    return 0; //No están ni cerca
}




// CHECKEA SI ESTA EN EL AREA DE UNA ABEJA 
int proximoAbeja(coor ndl, coor abeja){

    if((abeja.x-1 <= ndl.x && abeja.x+3 >= ndl.x) && (abeja.y+1 >= ndl.y && abeja.y-1 <= ndl.y))
        return 1; //ndl está en el area de la abeja

    return 0; //No están ni cerca
}




// DEVUELVE UNA UBI RANDOM DENTRO DE LOS LIMITES
coor randomUbi(coor abeja, char limit[maxV][maxH]){ 
	// srand(time(NULL));//Sirve para q rand() no de siempre los mismos numeros randoms
    coor nuevaUbi;
    
    nuevaUbi.x = abeja.x + (rand() % 3) - 1;
    nuevaUbi.y = abeja.y + (rand() % 3) - 1;

    // si nuevaUbi en calleLimit es un # entonces retrun abeja
    if(limit[maxV - nuevaUbi.y][nuevaUbi.x] == '#') {
        return abeja;
    } else {
        return nuevaUbi;
    }

}




void ubicarPivote(){
    // Vuelve al comienzo del terminal
	printf("\e[%iA", 100);

    // Margin top
    printf("\n\n\n\n\n");

    // Altura de los frames.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}



















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Estas funciones son solo para debuguear. //
void debugString(char errorMsj[100]){
    FILE *errors;
	errors = fopen("errors.txt", "a");
	fprintf(errors, errorMsj);
	fprintf(errors, "\n");
	fclose(errors);
}

void debugStringInt(char msj[100], int a){
    FILE *errors;

	errors = fopen("errors.txt", "a");
	fprintf(errors, "%s %d", msj, a);
	fprintf(errors, "\n");
	fclose(errors);
}

void debugChar(char a){
    FILE *errors;
	errors = fopen("errors.txt", "a");
	fprintf(errors, "%c", a);
	fprintf(errors, "\n");
	fclose(errors);
}

void debugStringCoor(char msj[100], coor ubi){
    FILE *errors;

	errors = fopen("errors.txt", "a");
	fprintf(errors, "%s : %d %d", msj, ubi.x, ubi.y);
	fprintf(errors, "\n");
	fclose(errors);
}

void debugEscena(char escena[maxV][maxH]){
    FILE *errors;
    errors = fopen("errors.txt", "a");

    char *escenaFila[maxH];

    for(int i = 0; i < maxV; i++){

        // escenaFila = &escena[0][i];
        escenaFila[0] = &escena[i][0];

        fprintf(errors, "%.62s", escenaFila[0]);
        fprintf(errors, "\n");
    }

    fclose(errors);
}

void debugNdlData(Player *ndl){
    FILE *errors;

	// errors = fopen("errors.txt", "a");
	errors = fopen("errors.txt", "w");  // Modo "w" sobrescribe el archivo

	fprintf(errors, "%s %d \n", "cantFlores:        ", ndl->cantFlores);
	fprintf(errors, "%s %d \n", "misionesCumplidas: ", ndl->misionesCumplidas);
	fprintf(errors, "%s %d \n", "primeraVez:        ", ndl->primeraVez);
	fprintf(errors, "%s %d \n", "lucides[0]:        ", ndl->lucides[0]);
	fprintf(errors, "%s %d \n", "lucides[1]:        ", ndl->lucides[1]);
	fprintf(errors, "%s %d \n", "lucides[2]:        ", ndl->lucides[2]);
	fprintf(errors, "%s %d \n", "lucides[3]:        ", ndl->lucides[3]);
	fprintf(errors, "%s %d \n", "hp:                ", ndl->hp);
	fprintf(errors, "%s %d \n", "ubi x:             ", ndl->ubi.x);
	fprintf(errors, "%s %d \n", "ubi y:             ", ndl->ubi.y);
	fprintf(errors, "%s %d \n", "calleLoc:          ", ndl->calleLoc);
	fprintf(errors, "%s %d \n", "colorL:            ", ndl->colorL);
    
	// fprintf(errors, "\n");
	fclose(errors);
}