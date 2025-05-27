#include "ingame.h"

int main() {
	srand(time(NULL));	//Sirve para q rand() no de siempre los mismos numeros randoms
						// todavia no probe si es necesario tenerlo aca

    char op;

	cls();
	
	printf("\e[?25l");	//ESCONDE EL "CURSOR" (PARA SIEMPRE)

    // MAIN MENU
	do{
        // system("cls");
		// cls();

		//DEVUELVE EL CURSOR AL PRINCIPIO DEL TERMINAL
		printf("\e[%iA", 600);

		// printf("-----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n\n");
		if (access("save.txt", F_OK) == 0) {
	        printf("\n\n\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\t\t 4.GUARDAR PARTIDA\n\t\t 5.BORRAR PARTIDA\n\n\n\n");
		} else {
			printf("\n\n\t\t   MENU\n\n\t\t 1.JUGAR\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\n\n\n\n\n");
		}



		// printf("\ntu ch es: %c", op);
		printf("\n\n\n                                                                                                                        ");
		printf("\n                                                     CONTROLES                                                          ");
		printf("\n                                                                                                                        ");
		printf("\n                                  MOVIMIENTO:                        SELECCIONAR:                                       ");
		printf("\n                               W                                                                                        ");
		printf("\n                            A  S  D    /    FLECHAS                BARRA ESPACIADORA                                    ");

		// printf("\n\n\n\n                                                                                                                       A");
		// printf("\n\n\n\n-----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n                                                                                                                       +");
		op = getch();
		switch(op){
			case '1':
				// COMENZAR A JUGAR
                playGame();
				break;
				
			case '2':
				// ABRIR CONFIG.
				// cls();
				break;

			case '3':
				// CERRAR EL JUEGO
				cls();
                return 0;
				break;

			case '4':
				// GUARDAR PARTIDA
				guardarPartida(&NDL);
				break;

			case '5':
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