#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

#define max 16
#define max2 60
#define SIZE 200


typedef struct {
	int x;
	int y;
} coor;

// int proximo(coor u, coor e);









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
		
    }
    else{
        printf("error");
    }

	fclose(f);
}










// CHECKEA EL ESTADO ENTRE LAS DOS UBICACIONES 
int proximo(coor u, coor e){
    
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















//  PRINTEA DIALOGO DE NPC
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
//                 ubi actual    input del                                      casa = 0
//                 de NDL        teclado        escLimitCalle/Casa              calle = 1
coor movimiento (char input, coor actualUbi, char movimientoLimit[max][max2], int casaOCalle) {
    coor movimientoAux = actualUbi;

    switch (casaOCalle){
        // CUANDO ESTAS EN CASA
        case 0:
        
            switch (input){
                case 'd':
                case 'D':
                    movimientoAux.x = movimientoAux.x + 3;
                    break;
                case 'a':
                case 'A':
                    movimientoAux.x = movimientoAux.x - 3;
                    break;
                case 'w':
                case 'W':
                    movimientoAux.y = movimientoAux.y + 1;
                    break;
                case 's':
                case 'S':
                    movimientoAux.y = movimientoAux.y - 1;
                    break;
                default:
                    break;
            }
            break;
        
        // CUANDO ESTAS EN LA CALLE
        case 1:

            switch (input){
                case 'd':
                case 'D':
                    movimientoAux.x = movimientoAux.x + 2;
                    break;
                case 'a':
                case 'A':
                    movimientoAux.x = movimientoAux.x - 2;
                    break;
                case 'w':
                case 'W':
                    movimientoAux.y = movimientoAux.y + 1;
                    break;
                case 's':
                case 'S':
                    movimientoAux.y = movimientoAux.y - 1;
                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }


    if(movimientoLimit[max - movimientoAux.y][movimientoAux.x] == '#' 
        // ||
        // movimientoAux.y<0
    ) {
        return actualUbi;
    } else {
        return movimientoAux;
    }
}

















//  
void cinematica(int l/*line*/){
	FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    int inicio = 0;
    char linea[max2+1];
    
    f = fopen("cinematicas.txt", "r");
    
    while(inicio < l){
        fscanf(f, "%c", &aux);
		fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        fscanf(f, "%c", &aux);
        inicio++;
    }

	system("cls");
    // printf("\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n");
    
    if(f != NULL){
        for (i=1; i<=max*2-2; i++){
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

// void cinematica2(int l/*line*/){
// 	FILE *f;
//     int i = 0;
//     int j = 0;
//     char aux;
//     int inicio = 0;
//     char linea[max2+1];
    
//     f = fopen("cinematicas.txt", "r");
    
//     while(inicio < l){
//         fscanf(f, "%c", &aux);
// 		fscanf(f, "%c", &aux);
//         fgets(linea, sizeof(linea), f);
//         fscanf(f, "%c", &aux);
//         inicio++;
//     }

// 	system("cls");
//     printf("\n\n\n\n\n\n\n");
    
//     if(f != NULL){
// 			fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			printf("                            ");
// 			printf("%s", linea);


//         for (i=1; i<=(max/2)-1; i++){
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			// printf("\n%s", linea);
// 			printf("\n");

//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			printf("\n                            ");
// 			printf("%s", linea);

//         }
//         printf("\n");
//     }
//     else{
//         printf("error");
//     }

// 	fclose(f);
// }

// void cinematica3(int l/*line*/){
// 	FILE *f;
//     int i = 0;
//     int j = 0;
//     char aux;
//     int inicio = 0;
//     char linea[max2+1];
    
//     f = fopen("cinematicas.txt", "r");
    
//     while(inicio < l){
//         fscanf(f, "%c", &aux);
// 		fscanf(f, "%c", &aux);
//         fgets(linea, sizeof(linea), f);
//         fscanf(f, "%c", &aux);
//         inicio++;
//     }

// 	system("cls");
//     printf("\n\n\n\n\n\n\n");
    
//     if(f != NULL){
// 			fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			printf("                            ");
// 			printf("");


//         for (i=1; i<=(max/3)-1; i++){

//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			// printf("\n%s", linea);
// 			printf("\n");
            
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			printf("\n                            ");
// 			printf("%s", linea);

//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fscanf(f, "%c", &aux);
//             fgets(linea, sizeof(linea), f);
// 			// printf("\n%s", linea);
// 			printf("\n");

//         }
//     }
//     else{
//         printf("error");
//     }

// 	fclose(f);
// }