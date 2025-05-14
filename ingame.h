#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
//#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

#include "auxFuncs.h"
#define max 16
#define max2 60

coor entyCoor[20];

// struct NoviaDeLisa {
// 	int HP; // NDL.HP
// 	int cantFlores;
// 	int misionesCumplidas;
// 	int primeraVez;
// 	int lucides[4];
// 	coor ubi;
// 	int calleLoc;
// };
struct NoviaDeLisa NDL;
// int actualLocation = 1;

coor flor;
coor puertaACalle;
coor curas;

char escCasa[max][max2];
char escCasaLimit[max][max2];
char escCalle[max][max2];
char escCalleLimit[max][max2];
// char casaDeVecino[max][max2];
// FUNCIONES
int menuDeCasa();
int menuDeCalle();
int menuDeVecino();

int dibujarEscena(char d[max][max2], int loc);

//lo uso para cargar la data guardada en el pj
//	deberia estar en el main pero para eso tengo q arreglar el tema de los punteros
char saveArray[11];



















// ESTA FUNCION TE VA A MOVER ENTRE DIFERENTES MENUS JUGABLES (CASA, CALLE, CASA DE VECINO)
void playGame() {

	/////////////////////////CARGA DE ESCENAS/////////////////
    /**/ cargaDeEscena(escCasa, 1);
	/**/ cargaDeEscena(escCasaLimit, 18);
	/**/ 
    /**/ cargaDeEscena(escCalle, 35);
	/**/ cargaDeEscena(escCalleLimit, 52);
	// /**/ cargaDeEscena(casaDeVecino, 120);
	/////////////////////////////////////////////////////////
	
	//////////////////INICIALISACION DE UBICACIONES////////////////// 
	/*		LOCS DENTRO DE LA CASA		*/
	/**/ NDL.ubi.x = 31;		//NDL por primera vez en el juego
    /**/ NDL.ubi.y = 6;
	/**/
    /**/ flor.x = 46;			//FLOR
    /**/ flor.y = 10;
    /**/
    /**/ puertaACalle.x = 56;	//PUERTA A LA CALLE
    /**/ puertaACalle.y = 6;
    /**/
    /**/ curas.x = 23;			//CURAS (HEALT)
    /**/ curas.y = 4;
	////////////////////////////////////////////////////////////////


	//	SI EXISTE EL ARCHIVO save.txt ENTONCES CARGA LOS DATOS EN EL PERSONAJE
	//		SINO INICIALIZA EL PERSONAJE DESDE 0
	if (access("save.txt", F_OK) == 0) {

		FILE *saveFile = fopen("save.txt", "r");

		//este if capaz es innecesario
		if (saveFile!=NULL){
			fgets(saveArray, sizeof(saveArray), saveFile);

			/////////////////CARGA DE PESONAJE GUARDADO/////////////////////
			/**/ NDL.HP = atoi(&saveArray[7]);
				char cAux = saveArray[0];
			/**/ NDL.cantFlores = atoi(&cAux); //0
				cAux = saveArray[1];
			/**/ NDL.misionesCumplidas = atoi(&cAux);
				cAux = saveArray[2];
			/**/ NDL.primeraVez = atoi(&cAux);
			/**/ for(int ini = 0; ini<=3; ini++){
					cAux = saveArray[3 + ini];
			/**/ 	NDL.lucides[ini] = atoi(&cAux);
			/**/ }
			/**/ NDL.calleLoc = 1;	//nuevo actual location
			////////////////////////////////////////////////////////////////

		}
	} else {
		
		/////////////////INICIALISACION DE PESONAJE/////////////////////
		/**/ NDL.HP = 100;
		/**/ NDL.cantFlores = 0; //0
		/**/ NDL.misionesCumplidas = 0;
		/**/ NDL.primeraVez = 0;
		/**/ for(int ini = 0; ini<=3; ini++){
		/**/ 	NDL.lucides[ini] = 0;
		/**/ }
		/**/ NDL.calleLoc = 1;	//nuevo actual location
		////////////////////////////////////////////////////////////////
		
		guardarPartida(&NDL);
	}

	/////////////////INICIALISACION DE ENTIDADES/////////////////////
	/**/ entyCoor[1].x= 30;			//abeja 1
	/**/ entyCoor[1].y= 7;
	/**/ entyCoor[2].x= 18;			//abeja 2
	/**/ entyCoor[2].y= 10;
	/**/ entyCoor[3].x= 45;			//abeja 3
	/**/ entyCoor[3].y= 4;
	/**/ entyCoor[10].x= 9;			//casa
	/**/ entyCoor[10].y= 6;
	/**/ entyCoor[11].x= 35;		//vecino 1
	/**/ entyCoor[11].y= 13;
	/**/ entyCoor[12].x= 31;		//vecino 2
	/**/ entyCoor[12].y= 12;
	/**/ entyCoor[13].x= 41;		//vecino 3
	/**/ entyCoor[13].y= 12;	
	/**/ entyCoor[14].x= 31;		//vecino 4
	/**/ entyCoor[14].y= 11;	
	////////////////////////////////////////////////////////////////


	// PRIMERO LLAMA AL MENU DE CASA
	int op = menuDeCasa();

	do{
		// CADA MENU DEVUELVE UN NRO QUE INDICA A Q NUEVA ESCENA/MENU VA EL PERSONAJE
		switch(op){
			case 1: {
				// menuDeCasa
				guardarPartida(&NDL);
				op = menuDeCasa();
				break;
			};
			case 2: {
				// menuDeCalle
				op = menuDeCalle();
				break;
			}
			case 3: {
				// menuDeVecino
				op = menuDeVecino();
				break;
			}
			default: {
                op = 7;
                break;
			}	

		};
	}while(op != 7);
}


















int menuDeCasa(){
    int estadoEnCasa = 0;
    	
        //Unica utilizacion de int primeraVez
		// CADA VEZ QUE PIERDAS TODA LA ENERGIA LISA TE MANDA UN MENSAJE
		// 		faltan checkeos pero creo q ya anda
			// if(NDL.HP < 100){
        	// 	estadoEnCasa = dibujarEscena(escCasa, 1);

			// 	switch (NDL.primeraVez){
			// 		case 0:
			// 			NDL.primeraVez++;
			// 			printf("[MENSAJE DE LISA]:\n\tDeje unos caramelos en la mesa para recuperar energia,\n\trecuerda que solo debes entregar las flores.\n");
			// 			freeze(6);
			// 			break;
			// 		case 1:
			// 			if(NDL.misionesCumplidas>1) {
			// 				NDL.primeraVez++;
			// 				printf("[MENSAJE DE LISA]:\n\tPorfa no te metas en problemas y solo entrega las flores.\n");
			// 				freeze(6);
			// 			}
			// 			break;
			// 		case 2:
			// 			if(NDL.misionesCumplidas>2) {
			// 				NDL.primeraVez++;
			// 				printf("[MENSAJE DE LISA]:\n\t....\n\t¿Por que haces esto?...\n");
			// 				freeze(6);
			// 			}
			// 			break;
			// 		default:
			// 			break;
			// 	}
			// }

    char casaInput = 'x';
    
    //system ("/bin/stty raw"); //FUNCIONA PARA Q GETCHAR() FUNCIONE EN UBUNTU
    
    while(casaInput != '.'){
        cls();
        // printf("Ubicacion: %d %d \n", NDL.ubi.x, NDL.ubi.y); //Ubicasion de la novia d lisa
        //printf("Ubicacion enemi: %d %d \n", eubi.x, eubi.y); //ubicasion de las abejas

		//	DEVUELVE 1 SI SALISTE A LA CALLE
		//	SINO DEVUELVE 0
        estadoEnCasa = dibujarEscena(escCasa, 1);
		// printf("\nmi save: %s", saveArray);
		// printf("\nmi save en 0: %c", saveArray[0]);
		// printf("\nmi save en 1: %c", saveArray[1]);
		// printf("\nmi save en 2: %c", saveArray[2]);
		// guardarPartida(&NDL);
        
        switch(estadoEnCasa){
            case 0:
                casaInput = getch();
                break;

			// SALIO A LA CALLE POR LA PUERTA
            case 1:

				guardarPartida(&NDL);

				// ESTE IF NO TE DEJA SALIR DE CASA CON 0 DE ENERGIA
				if(NDL.HP == 0) {
					NDL.ubi.x = NDL.ubi.x - 6;
				} else {
					NDL.ubi.x = 11;
					NDL.ubi.y = 6;
					NDL.calleLoc = 1;
					cargaDeEscena(escCalle, 1);
					cargaDeEscena(escCalleLimit, 18);
					return 2;
				}
                break;
				
            default:
                casaInput = getch();
                break;
        }
        
		// MOVIMIENTO DE NDL
		NDL.ubi = movimiento(casaInput, NDL.ubi, escCasaLimit, 0);
    }
}

















int menuDeCalle (){

	// CARGA LA CALLE A LA QUE LLEGASTE
	switch (NDL.calleLoc){
		case 1:
			cargaDeEscena(escCalle, 35);
			cargaDeEscena(escCalleLimit, 52);
			break;

		case 2:
			cargaDeEscena(escCalle, 69);
			cargaDeEscena(escCalleLimit, 86);
			break;

		case 3:
			cargaDeEscena(escCalle, 103);
			cargaDeEscena(escCalleLimit, 120);
			break;

		case 4:
			cargaDeEscena(escCalle, 137);
			cargaDeEscena(escCalleLimit, 154);
			break;	

		default:
			break;
	}

	//	REINICIA UBI DE ABEJAS CADA QUE VAS A LA CALLE
	// 		INCLUSO CUANDO CAMBIAS DE CALLE 
	/**/ entyCoor[1].x= 30;			//abeja 1
	/**/ entyCoor[1].y= 7;
	/**/ entyCoor[2].x= 18;			//abeja 2
	/**/ entyCoor[2].y= 10;
	/**/ entyCoor[3].x= 45;			//abeja 3
	/**/ entyCoor[3].y= 4;

	//system ("cls");
	int estadoEnCalle;
	char calleInput;

	do{
		cls();
		estadoEnCalle = dibujarEscena(escCalle, 2);
        // printf("Ubicacion: %d %d \n", NDL.ubi.x, NDL.ubi.y); //Ubicasion de la novia d lisa

		// printf("\nEstadoEnCalle: %d\n", estadoEnCalle);
        
		// printf("misionescumplidas: %d", NDL.misionesCumplidas);
		// printf("misionescumplidas: %d", NDL.HP);
		// printf("misionescumplidas: %d", NDL.cantFlores);

		// printf("NDL.calleLoc: %d", NDL.calleLoc);

		switch(estadoEnCalle){
			case 0:
				entyCoor[1] = randomUbi(entyCoor[1], escCalleLimit);
				entyCoor[2] = randomUbi(entyCoor[2], escCalleLimit);
				entyCoor[3] = randomUbi(entyCoor[3], escCalleLimit);
				break;
		    case 1:
				// ENTRAR A CASA POR PUERTA
				NDL.ubi.x = 52;
    			NDL.ubi.y = 6;
				return 1;
				break;
		    case 2:
				// TE ATRAPARON LAS ABEJAS, VOLVES A CASA
				cinematica(18);
				freeze(3);

				NDL.cantFlores = 0;
				NDL.ubi.x = 31;
    			NDL.ubi.y = 6;
				NDL.HP = NDL.HP - 25; //iba a chekear q tuviera mas de 0 hp pero directamente no podes salir a la calle con 0 hp
				return 1;
				break;
		    case 3:
				// ENTRAR A UN VECINO
		        if(NDL.calleLoc-1 > NDL.misionesCumplidas){
		            //cls();
		            printf("\nAVISO: Debes entregas la flor anterior.\n");
		        }
		        else{
    		        if(NDL.calleLoc <= NDL.misionesCumplidas){
                        // NDL.ubi.y = NDL.ubi.y - 1; 
    	        	    printf("\nNOTA: Esta flor ya fue entregada!\n");
                		if (NDL.lucides[NDL.calleLoc-1] == 1){
							printf ("\t(alguien la entrego por ti)\n");
                		}
    	        	}
    		        else{
						// ENTRAS A LA CASA DEL VECINO
						return 3;
    	        	}
		        }
		        break;
			case 4:
				// CAMBIO DE CALLE
				return 2;
				break;
			default:
				break;
		}

		calleInput = getch();

		// MOVIMIENTO DE NDL
		NDL.ubi = movimiento(calleInput, NDL.ubi, escCalleLimit, 1);
		
	}while(estadoEnCalle != 6);
}














int menuDeVecino(){

	int contador = (6 * NDL.misionesCumplidas)+1;

	char vecinoInput;
	int vecinoContadorDeEscuchas = 0;

	int vecinoFocusOption = 1;
	char vecinoMensajeError[100] = "";

	int vecino2Escuchado = 0;
	int auxMisionesCumplidas = NDL.misionesCumplidas;

	do{

		// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
		if(NDL.cantFlores < 1){
			cinematica(35 + ((17*5) * auxMisionesCumplidas));	//dibujo(casaDeVecino, 3);	
		}else{
			cinematica(52 + ((17*5) * auxMisionesCumplidas));
		}
		
		if (strcmp(vecinoMensajeError, "") != 0){
			printf("%s", vecinoMensajeError);
			strcpy(vecinoMensajeError, "");
		} else {
			printf("\n");
		}
		

		// DIALOGO DE VECINO
		leerEscuchar(contador);


		switch (vecinoFocusOption){
			case 1:
				printf("\t\t\t      _          .n.  _                 /.  \\               |     |\n");
				printf("\t\t\t     |          C O D  |               / )  /               |  |  |\n");
				printf("\t\t\t     | Dar Flor  *Y*   |     Escuchar .)(-{         Volver  |     |\n");
				printf("\t\t\t     |_           |   _|               \\_b                  |  |  |\n");
				break;

			case 2:
				printf("\t\t\t                 .n.        _           /.  \\_              |     |\n");
				printf("\t\t\t                C O D      |           / )  / |             |  |  |\n");
				printf("\t\t\t       Dar Flor  *Y*       | Escuchar .)(-{   |     Volver  |     |\n");
				printf("\t\t\t                  |        |_          \\_b   _|             |  |  |\n");
				break;

			case 3:
				printf("\t\t\t                 .n.                    /.  \\      _        |     | _\n");
				printf("\t\t\t                C O D                  / )  /     |         |  |  |  |\n");
				printf("\t\t\t       Dar Flor  *Y*         Escuchar .)(-{       | Volver  |     |  |\n");
				printf("\t\t\t                  |                    \\_b        |_        |  |  | _|\n");
				break;
			
			default:
				break;
		}

		switch (NDL.HP){
				case 100:
					printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [++++++++] \n");
					break;
				case 75:
					printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [++++++__] \n");
					break;
				case 50:
					printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [++++____] \n");
					break;
				case 25:
					printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [++______] \n");
					break;
				case 0:
					printf("\n\t\t\t\t\t\tENERGIA SOCIAL: [________] \n");
					break;
				default:
					break;
			}

		vecinoInput = getch();


		switch (vecinoInput)
		{
		case 'a':
		case 'A':
		case 'K':
			if (vecinoFocusOption > 1){
				vecinoFocusOption--;
			}		
			break;

		case 'd':
		case 'D':
		case 'M':
			if (vecinoFocusOption < 3){
				vecinoFocusOption++;
			}		
			break;

		case ' ':
			switch(vecinoFocusOption){
				case 1:
					// ESTE IF NO PERMITE ENTREGAR FLOR SIN FLOR EN MANO
					if(NDL.cantFlores < 1) {
						strcpy(vecinoMensajeError, "\n\tNo llevas flores contigo ahora!!!");
						// printf("No llevas flores contigo ahora!!!");
						// freeze(3);
					} else{
						cinematica(52 + ((17*5) * auxMisionesCumplidas));
						freeze(1);
						cinematica(69 + ((17*5) * auxMisionesCumplidas));
						freeze(1);
						cinematica(86 + ((17*5) * auxMisionesCumplidas));
						freeze(1);
						if (NDL.misionesCumplidas < 3){
							cinematica(511);	//animacion de la mano con la flor
						} else {
							cinematica(579);	//animacion de la mano con la flor con dedo vendado
						}
						freeze(1);
						cinematica(86 + ((17*5) * auxMisionesCumplidas));
						freeze(1);
						cinematica(103 + ((17*5) * auxMisionesCumplidas));
						freeze(2);
						NDL.misionesCumplidas++;
						NDL.cantFlores = 0;
						NDL.ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia ubicasion en la casa
						NDL.ubi.y = 6;
						// VOLVES A CASA
						return 1;
					}
					break;

				case 2:

					if(NDL.misionesCumplidas == 1){
						vecino2Escuchado = 1;
						auxMisionesCumplidas = 4;
					}

					if(NDL.cantFlores < 1) {
						strcpy(vecinoMensajeError, "\n\tERROR: Necesitas al menos una flor para poder interactuar!\t(Puedes conseguir una en casa)");

					// } else if(NDL.HP == 25){
						// strcpy(vecinoMensajeError, "\n\tERROR: Se te acabo la energia social!  .  :.\t(Puedes cargarla en casa)");

					} else{
						cls();
						NDL.HP = NDL.HP - 25;
						contador = contador + 1;
						vecinoContadorDeEscuchas = vecinoContadorDeEscuchas + 1;

						if (NDL.HP == 0){

							// ESTE IF CHECKEA SI ESCUCHASTE TODO
							if (vecinoContadorDeEscuchas == 4){
								NDL.lucides[NDL.misionesCumplidas] = 1;

								cinematica(52 + ((17*5) * (auxMisionesCumplidas-1)));
								printf("\n\n");
								leerEscuchar(contador);
								freeze(3);
							
							}

							NDL.misionesCumplidas = NDL.misionesCumplidas + 1;
							NDL.cantFlores = 0;
							NDL.ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia ubicasion en la casa
							NDL.ubi.y = 6;

							// deberia agregar una cinematica cuando te quedas sin energia 
							//		con un vecino pero no llegaste a las 4 escuchas

							// VOLVES A CASA
							return 1;
						}
					}
					break;
				
				case 3:
					NDL.ubi.y--;
					// VOLVES A LA CALLE
					return 2;
					break;
				
				default:
					break;
			}
			break;

		default:
			break;
		}

	}while(vecinoInput != '.');
}
















// ESTA FUNCION DIBUJA LA ESCENA Y DEVUELVE UN NRO DEPENDIENDO CON QUE HIZO CONTACTO
// 		SI NO HACE CONTACTO CON NADA DEVUELVE 0
int dibujarEscena(char d[max][max2], int loc){

    int j, i;
	int rAux;

    cls();
    // printf("\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n");
    switch(loc){
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
        case 1:											//DIBUJA LA ESCENA DE LA CASA

			// 		ESTOS IF CHEKEAN SI HACES CONTACTO CON ALGO
			// AGARRASTE UNA FLOR
            if(proximo(NDL.ubi, flor) == 1 && NDL.cantFlores == 0){
				NDL.cantFlores = 1;
				// freeze(1);
				if (NDL.misionesCumplidas < 3){
					cinematica(494);
					freeze(1);
					cinematica(511);
					freeze(1);
				} else {
					cinematica(562);
					freeze(1);
					cinematica(579);
					freeze(1);
				}
				cls();
			    // printf("\n\n\n\n\n\n\n");
			    printf("\n\n\n\n\n");
            }

			// COMISTE LOS CARAMELOS
            if(proximo(NDL.ubi, curas) == 1 && NDL.HP < 100){

				if (NDL.misionesCumplidas < 3){
					cinematica(596);
					freeze(1);
					cinematica(375);
					freeze(1);
				} else {
					cinematica(596);	//tengo q poner la mano con la venda aca
					freeze(1);
					cinematica(562);	//y aca
					freeze(1);
				}
				cls();
			    // printf("\n\n\n\n\n\n\n");
			    printf("\n\n\n\n\n");

				NDL.HP = 100;
            }

			// 		ESTE FOR SOLO DIBUJA LA ESCENA
            for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
					if(j == NDL.ubi.x && i == NDL.ubi.y+2){
						printf("--");
						j++;
					}else if(j == NDL.ubi.x-1 && i == NDL.ubi.y+1){
                        printf("./\\.");
						if(NDL.cantFlores > 0){
							printf("*");
							j++;
						}
                        j = j + 3;
                    }else if(j == NDL.ubi.x && i == NDL.ubi.y){
                        printf("!!");
						j++;
                    }else{
                        printf("%c", d[max-i][j]);
                    }
                }
                printf("\n");
            }

			//  (algunos if es mejor ponerlos desp de dibujar la escena)
			// 		ESTOS IF CHEKEAN SI HACES CONTACTO CON ALGO
			// SALISTE A LA CALLE POR LA PUERTA
			if(proximo(NDL.ubi, puertaACalle) == 1){
				return 1;
            }

			// DIBUJA CANTIDAD DE ENERGIA
			switch (NDL.HP){
				case 100:
					printf("\n\n\n\t\t\t\t\t\tENERGIA SOCIAL: [++++++++] \n");
					break;
				case 75:
					printf("\n\n\n\t\t\t\t\t\tENERGIA SOCIAL: [++++++__] \n");
					break;
				case 50:
					printf("\n\n\n\t\t\t\t\t\tENERGIA SOCIAL: [++++____] \n");
					break;
				case 25:
					printf("\n\n\n\t\t\t\t\t\tENERGIA SOCIAL: [++______] \n");
					break;
				case 0:
					printf("\n\n\n\t\t\t\t\t\tENERGIA SOCIAL: [________] \n");
					break;
				default:
					break;
			}

			printf("\e[?25l");	//ESCONDE EL "CURSOR"
            break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 2:											//DIBUJA LA ESCENA DE LA CALLE

			// 	ESTO ES PARA PASAR DE CALLE
			// 		AVANZAR DE CALLE
			if(NDL.ubi.x > 59){
				NDL.calleLoc++;
				// if(NDL.calleLoc <= NDL.misionesCumplidas + 1){		//todo lo comentado funciona, esta comentado para poder testear cosas en el mapa
					NDL.ubi.x = 1;
					return 4;
				// } else {
				// 	NDL.ubi.x = NDL.ubi.x - 2;
				// 	NDL.calleLoc--;
				// }
			}
            
			// 		VOLVER UNA CALLE
			if(NDL.ubi.x < 1){
				NDL.calleLoc--;
				// if (NDL.calleLoc >= 1){		//todo lo comentado funciona, esta comentado para poder testear cosas en el mapa
					NDL.ubi.x = 59;
					return 4;
				// } else {
				// 	NDL.ubi.x = NDL.ubi.x + 2;
				// 	NDL.calleLoc++;
				// }
			}

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
                    if((NDL.calleLoc == NDL.misionesCumplidas) && (NDL.misionesCumplidas > 0) && ((j == entyCoor[1].x) && (i == entyCoor[1].y) || (j == entyCoor[2].x) && (i == entyCoor[2].y) || (j == entyCoor[3].x) && (i == entyCoor[3].y))){
                        printf("GB-");
                        j = j + 2;
                    }else if((j == NDL.ubi.x) && (i == NDL.ubi.y)){
                        printf("L");
						if(NDL.cantFlores > 0){
							printf("*"); 
							j++;
						}
                    } else{
                        printf("%c", d[max-i][j]);
                    }
                }
                printf("\n");
            }

			// 		ESTOS IF CHEKEAN SI HACES CONTACTO CON ALGO
			// ENTRAR A CASA
            if(proximo(NDL.ubi, entyCoor[10])==2 && NDL.calleLoc==1){
                return 1;
            }

			// ATRAPADA POR ABEJA
            if(((NDL.calleLoc == NDL.misionesCumplidas)) && (NDL.cantFlores > 0 && NDL.misionesCumplidas > 0) && ((proximo(NDL.ubi, entyCoor[1]) > 0) || (proximo(NDL.ubi, entyCoor[2]) > 0) || (proximo(NDL.ubi, entyCoor[3]) > 0))){
                return 2;
            }

			// ENTRAR A VECINO 1 O 2
            if((proximo(NDL.ubi, entyCoor[11]) == 2 && NDL.calleLoc==1)
				|| (proximo(NDL.ubi, entyCoor[12]) == 2 && NDL.calleLoc==2)
				|| (proximo(NDL.ubi, entyCoor[13]) == 2 && NDL.calleLoc==3)
				|| (proximo(NDL.ubi, entyCoor[14]) == 2 && NDL.calleLoc==4)
			){
                return 3;
            }

			printf("\e[?25l");	//ESCONDE EL "CURSOR"
            break;

        default:
            break;
    }
    return 0;
}