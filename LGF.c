#include "ingame.h"

void optionsMenu(Player *data);
void deleteSaveMenu();

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

    // LAUNCH MENU
	do{
		//DEVUELVE EL CURSOR AL PRINCIPIO DEL TERMINAL
		printf("\e[%iA", 100);

		//DEVUELVE EL CURSOR AL PRINCIPIO DE LA LINEA
		printf("\e[%iA\r", 100);

		printf("+----------------------------------------------------------------------------------------------------------------------+");
		printf("\n\n\n\n\n\n\n");

		if (access("save.txt", F_OK) == 0) {
	        printf("\t\t   MENU\n\n\t\t 1.CONTINUAR  	\n\t\t \n\t\t 3.SALIR\n");
		} else {
			printf("\t\t   MENU\n\n\t\t 1.JUGAR\n\t\t \n\t\t 3.SALIR\n");
		}

		printf("\n\n\n\n\n\n\n\n");
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
                startGame(&actualGame);
				op = 7;
				break;
				
			case '2':
				// ABRIR CONFIG.
				// cls();
				// optionsMenu(&actualGame);
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
		
	int menuOptionFocus = 1;

    // INGAME MENU
	do{
		op = 'x';

		while (op != '\r' && op != ' ' && op != '\033'){
			//DEVUELVE EL CURSOR AL PRINCIPIO DEL TERMINAL
			printf("\e[%iA", 100);

			//DEVUELVE EL CURSOR AL PRINCIPIO DE LA LINEA
			printf("\e[%iA\r", 100);

			printf("+----------------------------------------------------------------------------------------------------------------------+");
			printf("\n\n\n\n\n\n\n");

			// if (access("save.txt", F_OK) == 0) {
			printf("\t    MENU\n\n ");
			printf("\t%s CONTINUAR \n ", 1 == menuOptionFocus ? "*" : "  ");
			printf("\t%s CONFIG \n ", 2 == menuOptionFocus ? "*" : "  ");
			printf("\t%s SALIR \n ", 3 == menuOptionFocus ? "*" : "  ");
			printf("\t%s GUARDAR PARTIDA \n ", 4 == menuOptionFocus ? "*" : "  ");
			printf("\t%s BORRAR PARTIDA \n", 5 == menuOptionFocus ? "*" : "  ");

			printf("\n\n\n\n\n\n");
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

			switch (op){
				case 'w':
				case 'W':
				case 72:
					if (menuOptionFocus > 1){
						menuOptionFocus--;
					} else{
						menuOptionFocus = 5;
					}
					
					break;

				case 's':
				case 'S':
				case 80:
					if (menuOptionFocus < 5){
						menuOptionFocus++;
					} else{
						menuOptionFocus = 1;
					}
					break;

				case '\033':
					menuOptionFocus = 1;
					break;
				
				default:
					break;
			}
		}
		

		switch(menuOptionFocus){
			case 1:
				// COMENZAR A JUGAR
				cls();
                startGame(&actualGame);
				break;
				
			case 2:
				// ABRIR CONFIG.
				optionsMenu(&actualGame);
				break;

			case 3:
				// CERRAR EL JUEGO
				cls();
				printf("\e[?25h");	//VUELVE A MOSTRAR EL "CURSOR"
                return 0;
				break;

			case 4:
				// GUARDAR PARTIDA
				guardarPartida(&noviaDeLisa, 1);
				break;

			case 5:
				// BORRAR PARTIDA GUARDADA
				deleteSaveMenu();
				break;

			default:
				// INGRESA UN VALOR ENTRE 1 Y 3
                break;
		};
	}while(menuOptionFocus != 7);
}




void optionsMenu(Player *data) {
    char colorNames[8][30]; // 8 strings, cada uno de hasta 19 caracteres + '\0'
    strcpy(colorNames[0], "\033[30mNegro\033[0m");
    strcpy(colorNames[1], "\033[31mRojo\033[0m");
    strcpy(colorNames[2], "\033[32mVerde\033[0m");
    strcpy(colorNames[3], "\033[33mAmarillo\033[0m");
    strcpy(colorNames[4], "\033[34mAzul\033[0m");
    strcpy(colorNames[5], "\033[35mMagenta\033[0m");
    strcpy(colorNames[6], "\033[36mCian\033[0m");
    strcpy(colorNames[7], "\033[37mBlanco\033[0m");

	char optionFocus = data->colorL - 30;
	char optionInput = 'x';

	while (optionInput != '\033' && optionInput != '.' && optionInput != '3' && optionInput != '2'){
		
		printf("\e[%iA\n", 100);
		printf("\n\n\n\n\n\n\n");
		for (int i = 7; i > 0; i--) {
			printf("\t\t\t\t\t\t\t\t\t\t\t\t\033[%dm%s\033[0m %s \n", data->colorL, i == optionFocus ? "*" : "  ", colorNames[i]);
		}

		optionInput = getch();

		switch (optionInput){
			case 'w':
			case 'W':
			case 72:
				if (optionFocus < 7){
					optionFocus++;
				} else {
					optionFocus = 1;
				}
				break;

			case 's':
			case 'S':
			case 80:
				if (optionFocus > 1){
					optionFocus--;
				} else {
					optionFocus = 7;
				}
				break;

			case ' ':
			case '\r':
				if (data->colorL == 30 + optionFocus){
					optionInput = '.';
				}
				data->colorL = 30 + optionFocus;
				guardarPartida(&actualGame, 2);
				break;
			
			default:
				break;
		}
	}

	printf("\e[%iA\n", 100);
	printf("\n\n\n\n\n\n\n");
	for (int i = 7; i > 0; i--) {
		printf("\t\t\t\t\t\t\t\t\t\t\t\t             \n");
	}
}




void deleteSaveMenu(){
	bool isSure = false;
	char optionInput = 'x';

	while (optionInput != '\033' && optionInput != '\r' && optionInput != ' ' && optionInput != '.' && optionInput != '3' && optionInput != '2'){
		
		printf("\e[%iA\n", 100);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("\t      BORRAR?\n\n");
		printf("\t   %s NO \n", !isSure ? "*" : "  ");
		printf("\t   %s SI \n", isSure ? "*" : "  ");

		optionInput = getch();

		switch (optionInput){
			case 'w':
			case 'W':
			case 72:
				isSure = !isSure;
				break;

			case 's':
			case 'S':
			case 80:
				isSure = !isSure;
				break;
			
			default:
				break;
		}
	}

	printf("\e[%iA\n", 100);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\t             \n\n");
	printf("\t         \n");
	printf("\t         \n");

	if ((optionInput == '\r' || optionInput == ' ') && isSure){
		cls();
		if (access("save.txt", F_OK) == 0) {
			remove("save.txt");
		}
		actualGame.ubi.y = 0;
	}
}