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
	srand(time(NULL));	//Sirve para q rand() no de siempre los mismos numeros randoms
						// todavia no probe si es necesario tenerlo aca

    char op;

    // MAIN MENU
	do{
        system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");		
        printf("\n\nOPCIONES\n 1.JUGAR\n 2.CONFIG\n 3.SALIR");
		printf("\e[?25l");	//ESCONDE EL "CURSOR"
		op = getch();
		switch(op){
			case '1':
				// COMENZAR A JUGAR
                playGame();
				break;
				
			case '2':
				// ABRIR CONFIG.
				break;

			case '3':
				// CERRAR EL JUEGO
                return 0;

			default:
				// INGRESA UN VALOR ENTRE 1 Y 3
                break;
		};
	}while(op != 7);

}