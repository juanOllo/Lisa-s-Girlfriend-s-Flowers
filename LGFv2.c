#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
//#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

#include "ingame.h"

int main() {
	srand(time(NULL));	//Sirve para q rand() no de siempre los mismos numeros randoms
						// todavia no probe si es necesario tenerlo aca

    char op;



	// FILE *save;
	// save = fopen("save.txt", "r");
	// fclose(save);

	// fprintf(save, "1000000025");
	// fclose(save);



    // MAIN MENU
	do{
        // system("cls");
		cls();
		printf("-----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n\n");
		if (access("save.txt", F_OK) == 0) {
	        printf("\n\n\t\t\t\t\t\tOPCIONES\n\n\t\t\t\t\t\t\t 1.CONTINUAR\n\t\t\t\t\t\t\t 2.CONFIG\n\t\t\t\t\t\t\t 3.SALIR\n\t\t\t\t\t\t\t 4.BORRAR PARTIDA\n\n\n\n\n");
		} else {
			printf("\n\n\t\t\t\t\t\tOPCIONES\n\n\t\t\t\t\t\t\t 1.JUGAR\n\t\t\t\t\t\t\t 2.CONFIG\n\t\t\t\t\t\t\t 3.SALIR\n\n\n\n\n\n");
		}



		printf("\e[?25l");	//ESCONDE EL "CURSOR"
		// printf("\ntu ch es: %c", op);
		printf("\n\n\n                                                                                                                        ");
		printf("\n                                                     CONTROLES                                                          ");
		printf("\n                                                                                                                        ");
		printf("\n                                  MOVIMIENTO:                        SELECCIONAR:                                       ");
		printf("\n                               W                                                                                        ");
		printf("\n                            A  S  D    /    FLECHAS                BARRA ESPACIADORA                                    ");

		// printf("\n\n\n\n                                                                                                                       A");
		printf("\n\n\n\n-----------------------------------------------------------------------------------------------------------------------+");
		op = getch();
		switch(op){
			case '1':
				// COMENZAR A JUGAR
                playGame();
				break;
				
			case '2':
				// ABRIR CONFIG.
				cls();
				break;

			case '3':
				// CERRAR EL JUEGO
				cls();
                return 0;

			case '4':
				// BORRAR PARTIDA GUARDADA
				cls();
				if (access("save.txt", F_OK) == 0) {
					remove("save.txt");
				}

				break;

			default:
				// INGRESA UN VALOR ENTRE 1 Y 3
                break;
		};
	}while(op != 7);

}