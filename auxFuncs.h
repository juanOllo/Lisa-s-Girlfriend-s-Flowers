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

// #define max 16
#define max 17
// #define max2 60
#define max2 62
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
} Player;


// No es necesario pero las defino aca para encontrarlas mas rapido
void actualizarHP(int hp);  
void actualizarMenuVecino(int vecinoFocusOption);
void cambiarCalleAnim(char escena[max][max2], char escena2[max][max2], char direccion);
void cargarEscenas(char escenaVisual[max][max2], char escenaLimites[max][max2], int l);
void cargarEscenasConAlternativa(char escenaVisual[max][max2], char escenaVisualAlternativa[max][max2], char escenaLimites[max][max2], int l);
void guardarPartida(Player *ndl, int typeSave);
void leerEscuchar(int cont);
void matrisDebug(char matris[max][max2]);
coor movimiento2(coor actualUbi, char movimientoLimit[max][max2], int casaOCalle);
coor movimientoConInput(char input, coor actualUbi, char movimientoLimit[max][max2], int casaOCalle);
int proximo(coor u, coor e);
int proximoAbeja(coor ndl, coor abeja);
coor randomUbi(coor e, char limit[max][max2]);
void ubicarPivote();




// Esta funcion esta arriba siempre porq la uso solo para debuggear y suelo cambiar el perfil y parametros.
void error1(char errorMsj[100]){
    FILE *errors;
	errors = fopen("errors.txt", "a");
	fprintf(errors, errorMsj);
	fprintf(errors, "\n");
	fclose(errors);
}

void error2(char msj[100], int a){
    FILE *errors;

	errors = fopen("errors.txt", "a");
	fprintf(errors, "%s %d", msj, a);
	fprintf(errors, "\n");
	fclose(errors);
}

void error3(char a){
    FILE *errors;
	errors = fopen("errors.txt", "a");
	fprintf(errors, "%c", a);
	fprintf(errors, "\n");
	fclose(errors);
}

void debugEscena(char escena[max][max2]){
    FILE *errors;
    errors = fopen("errors.txt", "a");

    char *escenaFila[max2];

    for(int i = 0; i < max; i++){

        // escenaFila = &escena[0][i];
        escenaFila[0] = &escena[i][0];

        fprintf(errors, "%.62s", escenaFila[0]);
        fprintf(errors, "\n");
    }

    fclose(errors);
}

void ndlDataDebug(Player *ndl){
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
    
	// fprintf(errors, "\n");
	fclose(errors);
}




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
void cambiarCalleAnim(char escena[max][max2], char escena2[max][max2], char direccion){
	printf("\e[%iA", 15);

    int k = 1;

    switch (direccion){
        // DERECHA
        case 'd':
            while (k < max2-1){
                
                for(int i = max-1; i > 1; i--){
                    printf("                              ");
                    for(int j = k; j < max2-1; j++){
                        printf("%c", escena[max-i][j]);
                    }

                    for (int d = 1; d < (k+1); d++){
                        printf("%c", escena2[max-i][d]);
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
            while (k < max2-2){
                
                for(int i = max-1; i > 1; i--){
                    printf("                              ");
                    // Escena a la q voy
                    for (int d = ((max2-2)-k); d < (max2-1); d++){
                        printf("%c", escena2[max-i][d]);
                    }

                    // Escena en la q estoy
                    for(int j = 1; j < (max2-2-k); j++){
                        printf("%c", escena[max-i][j]);
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
void cargarEscenas(char escenaVisual[max][max2], char escenaLimites[max][max2], int l){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    char linea[max2+2];
    
    f = fopen("escenas.txt", "r");
    
    while(i < l-1){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        i++;
    }
    
    if(f != NULL){

        // Carga la escena que se va a mostrar en pantalla.
        for (i = 0; i < max; i++){
            for(j = 0; j < max2; j++){
                fscanf(f, "%c", &escenaVisual[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que va a marcar los limites.
        for (i = 0; i <= max; i++){
            for(j = 0; j < max2; j++){
                fscanf(f, "%c", &escenaLimites[i][j]);
            }
            fscanf(f, "%c", &aux);
        }
		
    } else {
        printf("error");
    }

	fclose(f);
}




void cargarEscenasConAlternativa(char escenaVisual[max][max2], char escenaVisualAlternativa[max][max2], char escenaLimites[max][max2], int l){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    char linea[max2+2];
    
    f = fopen("escenas.txt", "r");
    
    while(i < l-1){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        i++;
    }
    
    if(f != NULL){

        // Carga la escena que se va a mostrar en pantalla.
        for (i = 0; i < max; i++){
            for(j = 0; j < max2; j++){
                fscanf(f, "%c", &escenaVisual[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que se va a alternar visualmente con la primera.
        for (i = 0; i < max; i++){
            for(j = 0; j < max2; j++){
                fscanf(f, "%c", &escenaVisualAlternativa[i][j]);
            }
            fscanf(f, "%c", &aux);
        }

        // Carga la escena que va a marcar los limites.
        for (i = 0; i <= max; i++){
            for(j = 0; j < max2; j++){
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

    char toSaveArray[16];

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

    // Guardo la informacion de toSaveArray en save.txt
    FILE *save;
	save = fopen("save.txt", "w");
	fprintf(save, toSaveArray);
	fclose(save);

    if (typeSabe == 1){
        printf("\e[%iA", 1);
        printf("\n\t+ [GUARDANDO PARTIDA] +");
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




void matrisDebug(char matris[max][max2]){

    FILE *errors;
	errors = fopen("errors.txt", "a");

    char *matrisFila[max2];

    for(int i = 0; i<20; i++){

        // matrisFila = &matris[0][i];
        matrisFila[0] = &matris[i][0];

        fprintf(errors, matrisFila[0]);
        fprintf(errors, "\n");
    }

	fclose(errors);
}




// FUNCION QUE DEVUELVE NUEVA UBI DE NDL CUANDO TE MOVES RESPETANDO LIMITES
//                    ubi actual                                    casa = 0
//                    de ndl        escLimitCalle/Casa/Vecino       calle = 1
//                                                                  vecino1 = 3
//                                                                  vecino2 = 4
coor movimiento2(coor actualUbi, char movimientoLimit[max][max2], int casaOCalle){

    coor nuevaUbiAux;
    // isOk sirve para q no redibuje la escena si no entro un input correcto
    char input;

     do{
        nuevaUbiAux = actualUbi;
        input = getch();

        switch (casaOCalle){
            case 0:
            case 1:
                switch (input){
                    case 'd':
                    case 'D':
                    case 'M':
                        nuevaUbiAux.x = nuevaUbiAux.x + (3 - casaOCalle);
                        break;
                    case 'a':
                    case 'A':
                    case 'K':
                        nuevaUbiAux.x = nuevaUbiAux.x - (3 - casaOCalle);
                        break;
                    case 'w':
                    case 'W':
                    case 'H':
                        nuevaUbiAux.y = nuevaUbiAux.y + 1;
                        break;
                    case 's':
                    case 'S':
                    case 'P':
                        nuevaUbiAux.y = nuevaUbiAux.y - 1;
                        break;
                    // case '.':
                    //     break;
                    default:
                        break;
                }
            break;

            case 3:
                switch (input){
                    case 'd':
                    case 'D':
                    case 'M':
                        nuevaUbiAux.x = nuevaUbiAux.x + 2;
                        break;
                    // case 'a':
                    // case 'A':
                    // case 'K':
                    //     nuevaUbiAux.x = nuevaUbiAux.x - 2;
                    //     break;
                    case 'w':
                    case 'W':
                    case 'H':
                        nuevaUbiAux.y = nuevaUbiAux.y + 2;
                        break;
                    case 's':
                    case 'S':
                    case 'P':
                        nuevaUbiAux.y = nuevaUbiAux.y - 2;
                        break;
                    // case '.':
                    //     break;
                    default:
                        break;
                }
            break;

            case 4:
                switch (input){
                    case 'd':
                    case 'D':
                    case 'M':
                        nuevaUbiAux.x = nuevaUbiAux.x + 5;
                        break;
                    case 'a':
                    case 'A':
                    case 'K':
                        nuevaUbiAux.x = nuevaUbiAux.x - 5;
                        break;
                    case 'w':
                    case 'W':
                    case 'H':
                        nuevaUbiAux.y = nuevaUbiAux.y + 2;
                        break;
                    case 's':
                    case 'S':
                    case 'P':
                        nuevaUbiAux.y = nuevaUbiAux.y - 2;
                        break;
                    // case '.':
                    //     break;
                    default:
                        break;
                }
            break;

            case 5:
                switch (input){
                    case 'd':
                    case 'D':
                    case 'M':
                        nuevaUbiAux.x = nuevaUbiAux.x + 2;
                        break;
                    case 'a':
                    case 'A':
                    case 'K':
                        nuevaUbiAux.x = nuevaUbiAux.x - 2;
                        break;
                    case 'w':
                    case 'W':
                    case 'H':
                        nuevaUbiAux.y = nuevaUbiAux.y + 1;
                        break;
                    case 's':
                    case 'S':
                    case 'P':
                        nuevaUbiAux.y = nuevaUbiAux.y - 1;
                        break;
                    // case '.':
                    //     break;
                    default:
                        break;
                }
            break;
        
            default:
            break;
        }

    }while (movimientoLimit[max - nuevaUbiAux.y][nuevaUbiAux.x] == '#' || proximo(nuevaUbiAux, actualUbi) == 2);

    return nuevaUbiAux;
}




// FUNCION QUE DEVUELVE NUEVA UBI DE NDL CUANDO TE MOVES RESPETANDO LIMITES
//                 input del      ubi actual                                    casa = 0
//                 teclado        de ndl        escLimitCalle/Casa              calle = 1
//                                                                              vecino1 = 3
//                                                                              vecino2 = 4
coor movimientoConInput(char input, coor actualUbi, char movimientoLimit[max][max2], int casaOCalle) {
    coor nuevaUbiAux = actualUbi;
    // isOk sirve para q no redibuje la escena si no entro un input correcto
    int isOk;

    do{
        isOk = 1;

        switch (casaOCalle){
            default:
                switch (input){
                    case 'd':
                    case 'D':
                    case 'M':
                        nuevaUbiAux.x = nuevaUbiAux.x + (3 - casaOCalle);
                        break;
                    case 'a':
                    case 'A':
                    case 'K':
                        nuevaUbiAux.x = nuevaUbiAux.x - (3 - casaOCalle);
                        break;
                    case 'w':
                    case 'W':
                    case 'H':
                        nuevaUbiAux.y = nuevaUbiAux.y + 1;
                        break;
                    case 's':
                    case 'S':
                    case 'P':
                        nuevaUbiAux.y = nuevaUbiAux.y - 1;
                        break;
                    case '.':
                        break;
                    default:
                        isOk = 0;
                        input = getch();
                        break;
                }
            break;
        }
    // }while (actualUbi.x == nuevaUbiAux.x && actualUbi.y == nuevaUbiAux.y);
    }while (isOk == 0);
    

    if(movimientoLimit[max - nuevaUbiAux.y][nuevaUbiAux.x] == '#') {
        return actualUbi;
        
    } else {
        return nuevaUbiAux;
    }
}




// CHECKEA EL ESTADO ENTRE LAS DOS UBICACIONES 
int proximo(coor u, coor e){
    
    // es e.x+"4" porq lo hice pensando en el tamaño de las abejas creo
    //  capaz haya q cambiarlo si lo voy a usar para algo mas al return 2
    if(e.x==u.x && e.y==u.y)
        return 2; //Están en el mismo lugar 

    // if((e.x+4>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2))
    //     return 1; //U está en el area de e

    if((e.x+2>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2))
        return 1; //U está en el area de e

    return 0; //No están ni cerca
}




// CHECKEA SI ESTA EN EL AREA DE UNA ABEJA 
int proximoAbeja(coor ndl, coor abeja){

    if((abeja.x-1 <= ndl.x && abeja.x+3 >= ndl.x) && (abeja.y+1 >= ndl.y && abeja.y-2 <= ndl.y))
        return 1; //ndl está en el area de la abeja

    return 0; //No están ni cerca
}




//  DEVUELVE UNA UBI RANDOM DENTRO DE LOS LIMITES
coor randomUbi(coor abeja, char limit[max][max2]){ 
	// srand(time(NULL));//Sirve para q rand() no de siempre los mismos numeros randoms
    coor aux;
    
    aux.x = abeja.x + (rand() % 3) - 1;
    aux.y = abeja.y + (rand() % 3) - 1;

    // voy a hacer un if q si aux en calleLimit es un # entonces retrun abeja
    if(limit[max - aux.y][aux.x] == '#') {
        return abeja;
    } else {
        return aux;
    }

}




void ubicarPivote(){
	printf("\e[%iA", 100);

    // Margin top
    printf("\n\n\n\n\n");

    // Altura de los frames.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
