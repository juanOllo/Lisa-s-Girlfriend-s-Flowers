#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

#include "ingame.h"

int main() {
	srand(time(NULL));//Sirve para q rand() no de siempre los mismos numeros randoms

    // MAIN MENU
    int op = 0;
	do{
        system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");		
        printf("\n\nOPCIONES\n 1.JUGAR\n 2.CONFIG\n 3.SALIR");
        char opc = getch();
	    op = atoi(&opc);
		switch(op){
			case 1: {
                playGame();
				break;
			};
			case 2: {
                printf("config.");
				break;
			}
			case 3: {
                printf("salir.");
                return 0;
				// break;
			}
			default: {
				printf("Ingrese un valor entre 1 y 3.\n");
                break;
			}	

		};
	}while(op != 7);

}