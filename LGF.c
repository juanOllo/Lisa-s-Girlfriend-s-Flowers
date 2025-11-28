#include "ingame.h"

Player actualGame;

int main() {

	if (access("errors.txt", F_OK) == 0) {
		remove("errors.txt");
	}

	srand(time(NULL));	//Sirve para q rand() no de siempre los mismos numeros randoms
						// todavia no probe si es necesario tenerlo aca

    char op;

	cls();
	
	printf("\e[?25l");	//ESCONDE EL "CURSOR" (PARA SIEMPRE)

    // MAIN MENU
	do{
		// actualGame = noviaDeLisa;
		// error2("actualGame.hp:", actualGame.HP);

		//DEVUELVE EL CURSOR AL PRINCIPIO DEL TERMINAL
		printf("\e[%iA", 100);

		//DEVUELVE EL CURSOR AL PRINCIPIO DE LA LINEA
		printf("\e[%iA\r", 100);

		printf("+----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n\n");

		if (access("save.txt", F_OK) == 0) {
	        // printf("\n\n\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\t\t 4.GUARDAR PARTIDA\n\t\t 5.BORRAR PARTIDA\n\n\n\n");
	        printf("\n\n\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\n\n\n\n\n");
		} else {
			printf("\n\n\t\t   MENU\n\n\t\t 1.JUGAR\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\n\n\n\n\n");
		}

		printf("\n\n\n                                                                                                                        ");
		printf("\n                                                     CONTROLES                                                          ");
		printf("\n                                                                                                                        ");
		printf("\n                                  MOVIMIENTO:                        SELECCIONAR:                                       ");
		printf("\n                               W                                                                                        ");
		printf("\n                            A  S  D    /    FLECHAS                BARRA ESPACIADORA                                    ");
		printf("\n                                                                                                                        ");
		printf("\n                                                                                                                        ");
		printf("\n                                                                                                                        ");
		printf("\n+----------------------------------------------------------------------------------------------------------------------+");

		op = getch();

		switch(op){
			case '1':
				// COMENZAR A JUGAR
				cls();
                playGame(&actualGame);
				op = 7;
				break;
				
			case '2':
				// ABRIR CONFIG.
				// cls();
				break;

			case '3':
				// CERRAR EL JUEGO
				cls();
				printf("\e[?25h");	//VUELVE A MOSTRAR EL "CURSOR"
                return 0;
				break;

			default:
				// INGRESA UN VALOR ENTRE 1 Y 3
                break;
		};
	}while(op != 7);
		
	do{
		//DEVUELVE EL CURSOR AL PRINCIPIO DEL TERMINAL
		printf("\e[%iA", 100);

		//DEVUELVE EL CURSOR AL PRINCIPIO DE LA LINEA
		printf("\e[%iA\r", 100);

		printf("+----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n\n");

		if (access("save.txt", F_OK) == 0) {
	        printf("\n\n\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\t\t 4.GUARDAR PARTIDA\n\t\t 5.BORRAR PARTIDA\n\n\n\n");
	        // printf("\n\n\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\n\n\n\n\n");
		} else {
			printf("\n\n\t\t   MENU\n\n\t\t 1.JUGAR\n\t\t 2.CONFIG\n\t\t 3.SALIR\n\n\n\n\n\n");
		}

		printf("\n\n\n                                                                                                                        ");
		printf("\n                                                     CONTROLES                                                          ");
		printf("\n                                                                                                                        ");
		printf("\n                                  MOVIMIENTO:                        SELECCIONAR:                                       ");
		printf("\n                               W                                                                                        ");
		printf("\n                            A  S  D    /    FLECHAS                BARRA ESPACIADORA                                    ");
		printf("\n                                                                                                                        ");
		printf("\n                                                                                                                        ");
		printf("\n                                                                                                                        ");
		printf("\n+----------------------------------------------------------------------------------------------------------------------+");

		op = getch();

		switch(op){
			case '1':
				// COMENZAR A JUGAR
				cls();
                playGame(&actualGame);
				break;
				
			case '2':
				// ABRIR CONFIG.
				// cls();
				break;

			case '3':
				// CERRAR EL JUEGO
				cls();
				printf("\e[?25h");	//VUELVE A MOSTRAR EL "CURSOR"
                return 0;
				break;

			case '4':
				// GUARDAR PARTIDA
				guardarPartida(&noviaDeLisa, 1);
				freeze_ms(1500);
				break;

			case '5':
				// BORRAR PARTIDA GUARDADA
				cls();
				if (access("save.txt", F_OK) == 0) {
					remove("save.txt");
				}
				actualGame.ubi.y = 0;
				break;

			default:
				// INGRESA UN VALOR ENTRE 1 Y 3
                break;
		};
	}while(op != 7);

}
