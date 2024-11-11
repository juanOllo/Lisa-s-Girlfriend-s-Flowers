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


typedef struct {
	int x;
	int y;
} coor;

int proximo(coor u, coor e);









// CARGA LA MATRIZ CON UNA ESCENA DEL DOCUMENTO escenas.txt 
//              COMENZANDO EN LA LINEA int l
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










// CHECKEA SI LAS EL ESTADO ENTRE LAS DOS UBICACIONES 
int proximo(coor u, coor e){
    
    if((e.x+4>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2)){
        if(e.x==u.x && e.y==u.y){
            return 2; //Están en el mismo lugar 
        }
        return 1; //U está en el area de e pero no en el mismo lugar
    }
    return 0; //No están ni cerca
}










coor randomUbi(coor e){ //Le da una ubicasion random a e dentro del area permitida
    coor aux;
    
    aux.x = aux.x + (rand() % 4) - 1;
    aux.y = aux.y + (rand() % 4) - 1;

    // voy a hacer un if q si aux en calleLimit es un # entonces retrun e

    return aux;
}