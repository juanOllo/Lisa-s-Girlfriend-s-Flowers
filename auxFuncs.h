#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include <pthread.h>

// #include <assert.h>
#include <unistd.h>

// ESTOS AUX SON PARA QUE FUNCIONE getch() y cls()
//#include <conio.h>
#include "auxLibsWin.h"
// #include "auxLibsUbu.h"

#define max 16
#define max2 60
#define SIZE 200


typedef struct {
	int x;
	int y;
} coor;

struct Player {
	int HP;                 //puntos de hp de ndl
	int cantFlores;         //cantidad de flores en mano
	int misionesCumplidas;  //cantidad de flores entregadas
	int primeraVez;         //aux q voy a usar para las cinematicas creo
	int lucides[4];         //guarda las veces q gastaste todos los dialogos a cada vecino
	coor ubi;               //ubicacion actual de ndl
	int calleLoc;           //en que seccion del juego se encuentra ndl
};


// No es necesario pero las defino aca para encontrarlas mas rapido
void matrisDebug(char matris[max][max2]);
void cargaDeEscena(char d[max][max2], int l);
int proximo(coor u, coor e);
coor randomUbi(coor e, char limit[max][max2]);
void freeze(int s);
void leerEscuchar(int cont);
void error(int a);
coor movimientoConInput(char input, coor actualUbi, char movimientoLimit[max][max2], int casaOCalle);
coor movimiento2(coor actualUbi, char movimientoLimit[max][max2], int casaOCalle);
void cinematica(int l/*line*/);
void guardarPartida(struct Player *ndl);
void actualizarHP(int hp);  
void marginTop();
void actualizarMenuVecino(int vecinoFocusOption);











void marginTop(){
	printf("\e[%iA", 100);

    // Margin top
    printf("\n\n\n\n\n");

    // Altura de los frames.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
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














// CARGA LA MATRIZ CON UNA ESCENA DEL DOCUMENTO escenas.txt 
//              COMENZANDO EN LA LINEA l
void cargaDeEscena(char d[max][max2], int l){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    int inicio = 0;
    char linea[max2+2];
    
    f = fopen("escenas.txt", "r");
    
    while(inicio < l){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        fscanf(f, "%c", &aux);
        inicio++;
    }
    
    if(f != NULL){
		
        for (i=1; i<=max; i++){
			d[i][0] = '#';
			//d[i][max] = 'j';
            fscanf(f, "%c", &aux);
            for(j=1; j<=max2; j++){
                fscanf(f, "%c", &d[i][j]);
            }
            fscanf(f, "%c", &aux);
            fscanf(f, "%c", &aux);
        }
		j=0;
		while(j < max2){
            d[max][j] = '#';
            d[0][j] = '#';
            j++;
		}

        //lo usaba para solucionar lo de las esquinas de las escenas de la calle, ponele
        // d[max][1] = 'j';
		
    }
    else{
        printf("error");
    }

	fclose(f);

    // matrisDebug(d);
}










// CHECKEA EL ESTADO ENTRE LAS DOS UBICACIONES 
int proximo(coor u, coor e){
    
    // es e.x+"4" porq lo hice pensando en el tamaño de las abejas creo
    //  capaz haya q cambiarlo si lo voy a usar para algo mas al return 2
    if((e.x+4>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2)){
        if(e.x==u.x && e.y==u.y){
            return 2; //Están en el mismo lugar 
        }
        return 1; //U está en el area de e pero no en el mismo lugar
    }
    return 0; //No están ni cerca
}















//  DEVUELVE UNA UBI RANDOM DENTRO DE LOS LIMITES
coor randomUbi(coor e, char limit[max][max2]){ 
	// srand(time(NULL));//Sirve para q rand() no de siempre los mismos numeros randoms
    coor aux;
    
    aux.x = e.x + (rand() % 3) - 1;
    aux.y = e.y + (rand() % 3) - 1;

    // voy a hacer un if q si aux en calleLimit es un # entonces retrun e
    if(limit[max - aux.y][aux.x] == '#') {
        return e;
    } else {
        return aux;
    }

}















//  PAUSA LA PANTALLA s SEGUNDOS
void freeze(int s){
	while(s>0){
        clock_t stop = clock() + CLOCKS_PER_SEC;
        while(clock() < stop){}
        s--;
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











// void error(char errorMsj[100]){
void error(int a){
    FILE *errors;
	errors = fopen("errors.txt", "a");
	// fprintf(errors, errorMsj);
	fprintf(errors, "%d", a);
	fprintf(errors, "\n");
	fclose(errors);
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
    

    if(movimientoLimit[max - nuevaUbiAux.y][nuevaUbiAux.x] == '#' 
        // ||
        // nuevaUbiAux.y<0
    ) {
        // error("colision");
        return actualUbi;
    } else {
        return nuevaUbiAux;
    }
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
        
            default:
            break;
        }

    }while (movimientoLimit[max - nuevaUbiAux.y][nuevaUbiAux.x] == '#' || proximo(nuevaUbiAux, actualUbi) == 2);

    return nuevaUbiAux;
}
















// FUNCION QUE DIBUJA UNA CINEMATICA
void cinematica(int l/*line*/){

    // Con esto oculto la barra de hp durante las cinematicas
    printf("\n\n\t                                                                      \n");
    printf("\e[%iA", 3);


	FILE *f;
    char aux;
    int contador = 0;
    char linea[max2+1];
    
    f = fopen("cinematicas.txt", "r");
    
    while(contador < l){
        fscanf(f, "%c", &aux);
		fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        fscanf(f, "%c", &aux);
        contador++;
    }

    printf("\e[%iA", 15);
    
    if(f != NULL){
        for (contador = 1; contador <= max*2-2; contador++){
			// printf("                            ");
			printf("                              ");
			fscanf(f, "%c", &aux);
            fgets(linea, sizeof(linea), f);
			printf("%s", linea);
        }
    }
    else{
        printf("error");
    }

	fclose(f);
}
















// Guarda TODOS LOS DATOS DE NDL EN save.txt
void guardarPartida(struct Player *ndl){

    char toSaveArray[16];

    sprintf(&toSaveArray[0], "%d", ndl->cantFlores);
    sprintf(&toSaveArray[1], "%d", ndl->misionesCumplidas);
    sprintf(&toSaveArray[2], "%d", ndl->primeraVez);
    sprintf(&toSaveArray[3], "%d", ndl->lucides[0]);
    sprintf(&toSaveArray[4], "%d", ndl->lucides[1]);
    sprintf(&toSaveArray[5], "%d", ndl->lucides[2]);
    sprintf(&toSaveArray[6], "%d", ndl->lucides[3]);

    switch (ndl->HP){
        case 100:
            sprintf(&toSaveArray[7], "%d", ndl->HP);
            break;
        
        case 25:
        case 50:
        case 75:
            sprintf(&toSaveArray[7], "%d", 0);
            sprintf(&toSaveArray[8], "%d", ndl->HP);
            break;

        case 0:
            sprintf(&toSaveArray[7], "%d", 0);
            sprintf(&toSaveArray[8], "%d", 0);
            sprintf(&toSaveArray[9], "%d", ndl->HP);
            break;
        
        default:
            break;
    }

    if(ndl->ubi.x > 9){
        sprintf(&toSaveArray[10], "%d", ndl->ubi.x);
    } else {
        sprintf(&toSaveArray[10], "%d", 0);
        sprintf(&toSaveArray[11], "%d", ndl->ubi.x);
    }
    
    if(ndl->ubi.y > 9){
        sprintf(&toSaveArray[12], "%d", ndl->ubi.y);
    } else {
        sprintf(&toSaveArray[12], "%d", 0);
        sprintf(&toSaveArray[13], "%d", ndl->ubi.y);
    }

    sprintf(&toSaveArray[14], "%d", ndl->calleLoc);


    FILE *save;
	save = fopen("save.txt", "w");
	fprintf(save, toSaveArray);
	fclose(save);
}



















void actualizarHP(int hp){
    
    printf("\n\n\t                                      ");
    switch (hp){
        case 100:
            // printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [++++++++] \n");
            printf(" ENERGIA SOCIAL: [////////] \n");
            break;
        case 75:
            printf(" ENERGIA SOCIAL: [//////--] \n");
            break;
        case 50:
            printf(" ENERGIA SOCIAL: [////----] \n");
            break;
        case 25:
            printf(" ENERGIA SOCIAL: [//------] \n");
            break;
        case 0:
            printf(" ENERGIA SOCIAL: [--------] \n");
            break;
        default:
            break;
    }

    printf("\e[%iA", 3);
}














void actualizarMenuVecino(int vecinoFocusOption){
    switch (vecinoFocusOption){
        case 1:
            printf("\t\t\t\t      _          .n.  _                |     |    \n");
            printf("\t\t\t\t     |          C O D  |               |  |  |    \n");
            printf("\t\t\t\t     | Dar Flor  *Y*   |       Volver  |     |    \n");
            printf("\t\t\t\t     |_           |   _|               |  |  |    \n");
            break;

        case 2:
            printf("\t\t\t\t                 .n.          _        |     | _  \n");
			printf("\t\t\t\t                C O D        |         |  |  |  |  \n");
			printf("\t\t\t\t       Dar Flor  *Y*         | Volver  |     |  |  \n");
			printf("\t\t\t\t                  |          |_        |  |  | _|  \n");
            break;
        
        default:
            break;
    }
    printf("\e[%iA", 4);
}