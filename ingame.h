#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

#include "auxFuncs.h"
#define max 16
#define max2 60

coor entyCoor[20];

struct NoviaDeLisa {
	int HP; // NDL.HP
	int cantFlores;
	int misionesCumplidas;
	int primeraVez;
	int lucides[4];
	coor ubi;
	int calleLoc;
};
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


















// ESTA FUNCION TE VA A MOVER ENTRE DIFERENTES MENUS JUGABLES (CASA, CALLE, CASA DE VECINO)
void playGame() {

	/////////////////////////CARGA DE ESCENAS/////////////////
    /**/ cargaDeEscena(escCasa, 35);
	/**/ cargaDeEscena(escCasaLimit, 52);
	/**/ 
    /**/ cargaDeEscena(escCalle, 1);
	/**/ cargaDeEscena(escCalleLimit, 18);
	// /**/ cargaDeEscena(casaDeVecino, 120);
	/////////////////////////////////////////////////////////
	
	//////////////////INICIALISACION DE UBICACIONES////////////////// 
	/*		LOCS DENTRO DE LA CASA		*/
	/**/ NDL.ubi.x = 31;		//NDL por primera vez en el juego
    /**/ NDL.ubi.y = 6;
	/**/
    /**/ flor.x = 46;	//FLOR
    /**/ flor.y = 10;
    /**/
    /**/ puertaACalle.x = 56;	//PUERTA A LA CALLE
    /**/ puertaACalle.y = 6;
    /**/
    /**/ curas.x = 23;	//CURAS (HEALT)
    /**/ curas.y = 4;
	////////////////////////////////////////////////////////////////

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
	////////////////////////////////////////////////////////////////


    // int op = 0;
	// PRIMERO LLAMA AL MENU DE CASA
	int op = menuDeCasa();

	do{

		// CADA MENU DEVUELVE UN NRO QUE INDICA A Q NUEVA ESCENA VA EL PERSONAJE
		switch(op){
			case 1: {
				// menuDeCasa
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
    
    while(casaInput != 'p'){
        system("cls");
        // printf("Ubicacion: %d %d \n", NDL.ubi.x, NDL.ubi.y); //Ubicasion de la novia d lisa
        //printf("Ubicacion enemi: %d %d \n", eubi.x, eubi.y); //ubicasion de las abejas

		//	DEVUELVE 1 SI SALISTE A LA CALLE
		//	SINO DEVUELVE 0
        estadoEnCasa = dibujarEscena(escCasa, 1);
        
        switch(estadoEnCasa){
            case 0:
                casaInput = getch();
                break;

			// SALIO A LA CALLE POR LA PUERTA
            case 1:
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
			cargaDeEscena(escCalle, 1);
			cargaDeEscena(escCalleLimit, 18);
			break;

		case 2:
			cargaDeEscena(escCalle, 69);
			cargaDeEscena(escCalleLimit, 86);
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
	int estado;
	char calleInput;

	do{
		system("cls");
		estado = dibujarEscena(escCalle, 2);
        // printf("Ubicacion: %d %d \n", NDL.ubi.x, NDL.ubi.y); //Ubicasion de la novia d lisa

		// printf("\nEstado: %d\n", estado);
        
		// printf("misionescumplidas: %d", NDL.misionesCumplidas);
		// printf("misionescumplidas: %d", NDL.HP);
		// printf("misionescumplidas: %d", NDL.cantFlores);

		// printf("NDL.calleLoc: %d", NDL.calleLoc);

		switch(estado){
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
				NDL.cantFlores = 0;
				NDL.ubi.x = 31;
    			NDL.ubi.y = 6;
				return 1;
				break;
		    case 3:
				// ENTRAR A UN VECINO
		        if(NDL.calleLoc-1 > NDL.misionesCumplidas){
		            //system("cls");
		            printf("XXXX Debes entregar la flor anterior XXXX\n");
		        }
		        else{
    		        if(NDL.calleLoc <= NDL.misionesCumplidas){
                        // NDL.ubi.y = NDL.ubi.y - 1; 
    	        	    printf("\nEsta flor ya fue entregada\n");
                		if (NDL.lucides[NDL.calleLoc-1] == 1){
							printf ("(alguien la entrego por ti)\n");
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
		
	}while(estado != 6);
}














int menuDeVecino(){

	int contador = (6 * NDL.misionesCumplidas)+1;

	char vecinoInput;
	int vecinoContadorDeEscuchas = 0;

	do{

		// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
		if(NDL.cantFlores < 1){
			cinematica(35 + ((17*5) * NDL.misionesCumplidas));	//dibujo(casaDeVecino, 3);	
		}else{
			cinematica(103 + ((17*5) * NDL.misionesCumplidas));
		}
		
		// DIALOGO DE VECINO
		leerEscuchar(contador);

		//printf("\t\t\t\t1.Dar Flor\t2.Escuchar\t3.Salir a la calle\n");
		printf("\t\t\t                 .n.                    /.  \\               |     |\n");
		printf("\t\t\t                C O D                  / )  /               |  |  |\n");
		printf("\t\t\t    1 .Dar Flor  *Y*      2 .Escuchar .)(-{      3 .Volver  |     |\n");
		printf("\t\t\t                  |                    \\_b                  |  |  |\n");

		vecinoInput = getch();

		switch(vecinoInput){
			case '1':
				// ESTE IF NO PERMITE ENTREGAR FLOR SIN FLOR EN MANO
				if(NDL.cantFlores < 1) {
					printf("No llevas flores contigo ahora!!!");
					freeze(3);
				} else{
					// strcpy(msj, "*Entregaste la flor*\n");
					cinematica(103 + ((17*5) * NDL.misionesCumplidas));	//dibujo(casaDeVecino, 3);
					freeze(1);
					cinematica(52 + ((17*5) * NDL.misionesCumplidas));
					freeze(1);
					cinematica(69 + ((17*5) * NDL.misionesCumplidas));
					freeze(1);
					cinematica(86 + ((17*5) * NDL.misionesCumplidas));
					freeze(2);
					NDL.misionesCumplidas++;
					NDL.cantFlores = 0;
					NDL.ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia ubicasion en la casa
					NDL.ubi.y = 6;
					// VOLVES A CASA
					return 1;
				}
				break;

			case '2':
				if(NDL.cantFlores < 1) {
					printf("Necesitas al menos una flor para poder interactuar!\n(Puedes conseguir una en casa)");
					freeze(3);
				} else if(NDL.HP == 0){
					printf("Se te acabo la energia social!!!  .  :.\n(Puedes cargarla en casa)");
					freeze(3);
				} else{
					system("cls");
					NDL.HP = NDL.HP - 25;
					contador = contador + 1;
					vecinoContadorDeEscuchas = vecinoContadorDeEscuchas + 1;

					// ESTE IF CHECKEA SI ESCUCHASTE TODO
					if (vecinoContadorDeEscuchas == 4){
						NDL.lucides[NDL.misionesCumplidas] = 1;
						NDL.misionesCumplidas = NDL.misionesCumplidas + 1;
						NDL.cantFlores = 0;
						NDL.ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia ubicasion en la casa
						NDL.ubi.y = 6;

						cinematica(103 + ((17*5) * (NDL.misionesCumplidas-1)));
						leerEscuchar(contador);
						freeze(3);
						// VOLVES A CASA
						return 1;
					}
				}
				break;
			
			case '3':
				NDL.ubi.y--;
				// VOLVES A LA CALLE
				return 2;
				break;
			
			default:
				break;
		}
	}while(vecinoInput >= 0);
}
















// ESTA FUNCION DIBUJA LA ESCENA Y DEVUELVE UN NRO DEPENDIENDO CON QUE HIZO CONTACTO
// 		SI NO HACE CONTACTO CON NADA DEVUELVE 0
int dibujarEscena(char d[max][max2], int loc){

    int j, i;
	int rAux;

    system("cls");
    printf("\n\n\n\n\n\n\n");
    switch(loc){
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
        case 1:											//DIBUJA LA ESCENA DE LA CASA
			// ESTE FOR SOLO DIBUJA LA ESCENA
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

			// SALISTE A LA CALLE POR LA PUERTA
			if(proximo(NDL.ubi, puertaACalle) == 1){
				return 1;
            }

			// AGARRASTE UNA FLOR
            if(proximo(NDL.ubi, flor) == 1 && NDL.cantFlores == 0){
				NDL.cantFlores = 1;
            }

			// COMISTE LOS CARAMELOS
            if(proximo(NDL.ubi, curas) == 1 && NDL.HP < 100){
				NDL.HP = 100;
            }
            
			// printf("\n\n\n\t\t\t\t\t   <3 ENERGIA: %d <3", NDL.HP);
        	// printf("\tFLOR EN MANO: %d ,* \n\n", NDL.cantFlores);

			switch (NDL.HP){
			case 100:
				printf("\n\n\n\t\t\t\t\t\t    ENERGIA: [++++++++] \n");
				break;
			case 75:
				printf("\n\n\n\t\t\t\t\t\t    ENERGIA: [++++++__] \n");
				break;
			case 50:
				printf("\n\n\n\t\t\t\t\t\t    ENERGIA: [++++____] \n");
				break;
			case 25:
				printf("\n\n\n\t\t\t\t\t\t    ENERGIA: [++______] \n");
				break;
			case 0:
				printf("\n\n\n\t\t\t\t\t\t    ENERGIA: [________] \n");
				break;
			default:
				break;
			}

            break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 2:											//DIBUJA LA ESCENA DE LA CALLE
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
			){
                return 3;	
            }
            
			// 	ESTO ES PARA PASAR DE CALLE
			// 		AVANZAR DE CALLE
			if(NDL.ubi.x > 59){
				NDL.calleLoc++;
				NDL.ubi.x = 1;
				return 4;
			}
			// 		VOLVER UNA CALLE
			if(NDL.ubi.x < 1){
				NDL.calleLoc--;
				NDL.ubi.x = 59;
				return 4;
			}

            break;

        default:
            break;
    }
    return 0;
}