#include <stdio.h>
#include <time.h>

#define max 16
#define max2 60
#define SIZE 200

void cinematica(int l/*line*/, int mss);
void freeze_ms(int ms);



// FUNCIONES DE CINEMATICAS ESPECIFICAS
void agarrarFlorCinematica(){
    cinematica(766, 700);
    cinematica(783, 1000);
}

void entregar2daFlorCinematica(){
    cinematica(137, 1000);
    cinematica(392, 700);
    cinematica(409, 700);
    cinematica(426, 700);
    cinematica(443, 900);
}

void tomarCurasCinematica(){
    cinematica(800, 800);
    cinematica(817, 400);
    cinematica(834, 400);
    cinematica(766, 200);
}




//  PAUSA LA PANTALLA s MILISEGUNDOS
void freeze_ms(int ms) {
    clock_t start_time = clock();
    clock_t wait_time = ms * (CLOCKS_PER_SEC / 1000);
    while (clock() - start_time < wait_time) {}
}




// FUNCION QUE DIBUJA UNA CINEMATICA
//  l = linea de la cinemática en cinematicas.txt
//  mss = milisegundos que se pausa la pantalla al finalizar la cinemática
void cinematica(int l/*line*/, int mss){

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

    if (mss > 0)
        freeze_ms(mss);
}