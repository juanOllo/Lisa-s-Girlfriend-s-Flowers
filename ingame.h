#include "auxFuncs.h"

// noviaDeLisa es el/la personaje jugable
struct Player noviaDeLisa;

// Lista de coordenadas de algunas entidades
coor entyCoor[20];

coor flor;
coor puertaACalle;
coor curas;

// Matrices que guardan las escenas
char escCasa[max][max2];
char escCasaLimit[max][max2];
char escCalle[max][max2];
char escCalleLimit[max][max2];

char escVecino[max][max2];
char escVecinoLimit[max][max2];

// char casaDeVecino[max][max2];

// FUNCIONES
int menuDeCasa();
int menuDeCalle();
int menuDeVecino();

int vecinoGameplay();

int dibujarEscena(char d[max][max2], int loc);

//lo uso para cargar la data guardada en el pj
//	deberia estar en el main pero para eso tengo q arreglar el tema de los punteros
char saveArray[16];


// Obstaculos del juego de 1er vecino
coor obstaculos[5];
int velocidadObstaculos = 300;

// Balones del juego de 2do vecino
coor balones[6];
int velocidadBalones = 200;

float fps = 20; //frames por segundo
float fpsDibujo; 
int impactoConEntidad = 0;
volatile bool seguirDibujando = true;



















// ESTA FUNCION TE VA A MOVER ENTRE DIFERENTES MENUS JUGABLES (CASA, CALLE, CASA DE VECINO)
void playGame() {

	fpsDibujo = 1000 / fps; //milisegundos entre cada frame de dibujo

	///////////////////CARGA DE ESCENA DE LA CASA////////////
    /**/ cargaDeEscena(escCasa, 1);
	/**/ cargaDeEscena(escCasaLimit, 18);
	/////////////////////////////////////////////////////////

	//	SI EXISTE EL ARCHIVO save.txt ENTONCES CARGA LOS DATOS EN EL PERSONAJE
	//		SINO INICIALIZA EL PERSONAJE DESDE 0
	if (access("save.txt", F_OK) == 0) {
	// access(...) == 0: verifica que el archivo existe y es accesible.

		// fopen(...): intenta abrirlo, y puede fallar por otras razones 
		//	(por ejemplo, permisos insuficientes, bloqueo por otro proceso, corrupción, etc.).
		FILE *saveFile = fopen("save.txt", "r");
		// Por eso es buena idea verificar que fopen no devuelva NULL.
		if (saveFile!=NULL){
			fgets(saveArray, sizeof(saveArray), saveFile);

			/////////////////CARGA DE PESONAJE GUARDADO/////////////////////			
			/**/ char cAux = saveArray[0];
			/**/ noviaDeLisa.cantFlores = atoi(&cAux); //0
			/**/
			/**/ cAux = saveArray[1];
			/**/ noviaDeLisa.misionesCumplidas = atoi(&cAux);
			/**/
			/**/ cAux = saveArray[2];
			/**/ noviaDeLisa.primeraVez = atoi(&cAux);
			/**/
			/**/ for(int ini = 0; ini<=3; ini++){
			/**/	cAux = saveArray[3 + ini];
			/**/ 	noviaDeLisa.lucides[ini] = atoi(&cAux);
			/**/ }
			/**/
			/**/ noviaDeLisa.HP = atoi(&saveArray[7]) / 100000;
			/**/
			/**/ noviaDeLisa.ubi.x = atoi(&saveArray[10]) / 1000;
			/**/ noviaDeLisa.ubi.y = atoi(&saveArray[12]) / 10;	
			/**/
			/**/ cAux = saveArray[14];
			/**/ noviaDeLisa.calleLoc = atoi(&cAux);
			////////////////////////////////////////////////////////////////
		}

		fclose(saveFile);
		
	} else {
		
		/////////////////INICIALISACION DE PESONAJE/////////////////////
		/**/ noviaDeLisa.HP = 100;
		/**/ noviaDeLisa.cantFlores = 0;
		/**/ noviaDeLisa.misionesCumplidas = 0;
		/**/ noviaDeLisa.primeraVez = 0;
		/**/ for(int ini = 0; ini<=3; ini++){
		/**/ 	noviaDeLisa.lucides[ini] = 0;
		/**/ }
		/**/ noviaDeLisa.ubi = (coor){31, 6};	//noviaDeLisa por primera vez en el juego
		/**/ noviaDeLisa.calleLoc = 0;
		////////////////////////////////////////////////////////////////
		
		guardarPartida(&noviaDeLisa);
	}

	/////////////////INICIALISACION DE ENTIDADES/////////////////////
	/**/ flor = (coor){46, 10};
	/**/ puertaACalle = (coor){56, 6};
	/**/ curas = (coor){24, 4};
	/**/
	/**/ entyCoor[1] = (coor){30, 7};		//abeja 1
	/**/ entyCoor[2] = (coor){18, 10};		//abeja 2
	/**/ entyCoor[3] = (coor){45, 4};		//abeja 3
	/**/ entyCoor[10] = (coor){9, 6};		//casa
	/**/ entyCoor[11] = (coor){35, 13};		//vecino 1
	/**/ entyCoor[12] = (coor){31, 12};		//vecino 2
	/**/ entyCoor[13] = (coor){41, 12};		//vecino 3
	/**/ entyCoor[14] = (coor){31, 11};		//vecino 4
	////////////////////////////////////////////////////////////////

	int op;

	// INICIALIZA EL MENU EN EL Q DEBE ESTAR SEGUN LA UBICACION GUARDADA
	switch (noviaDeLisa.calleLoc){
		// CASA
		case 0:
			op = 1;
			break;
		
		// ALGUNA DE LAS CALLES
		default:
			op = 2;
			break;
	}

	do{
		// Se guarda partida apenas salis del menu principal
		// guardarPartida(&noviaDeLisa);

		// Cada que entro a un nuevo menu dejo el cursor en la ubicasion correcta.
		marginTop();

		// CADA MENU DEVUELVE UN NRO QUE INDICA A Q NUEVA ESCENA/MENU VA EL PERSONAJE
		switch(op){
			case 1:
				// menuDeCasa
				op = menuDeCasa();
				break;
			
			case 2:
				// menuDeCalle
				op = menuDeCalle();
				break;
			
			case 3:
				// menuDeVecino
				op = menuDeVecino();
				break;
			
			default:
                op = 7;
                break;
		}
	}while(op != 7);
}


















int menuDeCasa(){

	noviaDeLisa.calleLoc = 0;

    int estadoEnCasa = 0;

    char casaInput = 'x';
    
	// Ya no lo necesito pero lo guardo por las dudas.
    //system ("/bin/stty raw"); //FUNCIONA PARA Q GETCHAR() FUNCIONE EN UBUNTU
    
	actualizarHP(noviaDeLisa.HP);

    while(casaInput != '.'){
    // while(estadoEnCasa == 0){

		//	DEVUELVE 1 SI SALISTE A LA CALLE
		//	SINO DEVUELVE 0
        estadoEnCasa = dibujarEscena(escCasa, 1);
        
        switch(estadoEnCasa){
            case 0:
                // casaInput = getch();
				// noviaDeLisa.ubi = movimiento2(noviaDeLisa.ubi, escCasaLimit, 0);
                break;

			// SALIO A LA CALLE POR LA PUERTA.
            case 1:

				// ESTE IF NO TE DEJA SALIR DE CASA CON 0 DE ENERGIA.
				if(noviaDeLisa.HP == 0) {
					noviaDeLisa.ubi.x = noviaDeLisa.ubi.x - 6;

				} else {
					noviaDeLisa.ubi = (coor){11, 6};
					noviaDeLisa.calleLoc = 1;
					cargaDeEscena(escCalle, 1);
					cargaDeEscena(escCalleLimit, 18);
					return 2;
				}
                break;
				
			// AGARRASTE UNA FLOR.
			case 2:
				noviaDeLisa.cantFlores = 1;
				
				// if (noviaDeLisa.misionesCumplidas < 3){
					cinematica(494, 700);
					cinematica(511, 1000);
				// } else {
				// 	cinematica(562);
				// 	freeze(1);
				// 	cinematica(579);
				// 	freeze(1);
				// }

				estadoEnCasa = dibujarEscena(escCasa, 1);
				actualizarHP(noviaDeLisa.HP);
				break;

			// AGARRASTE UN CARAMELO.
			case 3:
				// noviaDeLisa.HP = 100;

				// if (noviaDeLisa.misionesCumplidas < 3){
					cinematica(698, 800);
					cinematica(715, 400);
					cinematica(732, 400);
					cinematica(494, 200);
				// } else {
				// 	cinematica(596);	//tengo q poner la mano con la venda aca
				// 	freeze(1);
				// 	cinematica(562);	//y aca
				// 	freeze(1);
				// }

				actualizarHP(noviaDeLisa.HP);
				freeze_ms(700);
				noviaDeLisa.HP = 100;
				actualizarHP(noviaDeLisa.HP);
				freeze_ms(500);

				estadoEnCasa = dibujarEscena(escCasa, 1);
				break;

            default:
                break;
        }

		casaInput = getch();
        
		// MOVIMIENTO DE noviaDeLisa
		noviaDeLisa.ubi = movimientoConInput(casaInput, noviaDeLisa.ubi, escCasaLimit, 0);
    }
}

















int menuDeCalle (){

	// Con esto oculto la barra de hp en la calle.
    printf("\n\n\t                                                                      \n");
    printf("\e[%iA", 3);


	// CARGA LA CALLE A LA QUE LLEGASTE
	switch (noviaDeLisa.calleLoc){
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

	//	REINICIA UBI DE ABEJAS CADA QUE LLEGAS A LA CALLE
	// 		INCLUSO CUANDO CAMBIAS DE CALLE 
	/**/ entyCoor[1] = (coor){30, 7};	//abeja 1
	/**/ entyCoor[2] = (coor){18, 10};	//abeja 2
	/**/ entyCoor[3] = (coor){45, 4};	//abeja 3
	
	int estadoEnCalle = dibujarEscena(escCalle, 2);
	char calleInput;

	do{
		estadoEnCalle = dibujarEscena(escCalle, 2);

		switch(estadoEnCalle){
			case 0:
				entyCoor[1] = randomUbi(entyCoor[1], escCalleLimit);
				entyCoor[2] = randomUbi(entyCoor[2], escCalleLimit);
				entyCoor[3] = randomUbi(entyCoor[3], escCalleLimit);
				break;

			// ENTRAR A CASA POR PUERTA
		    case 1:
				noviaDeLisa.ubi = (coor){52, 6};
				// noviaDeLisa.calleLoc = 0;
				return 1;
				break;

			// TE ATRAPARON LAS ABEJAS, VOLVES A CASA
		    case 2:
				cinematica(18, 3000);

				noviaDeLisa.cantFlores = 0;
				noviaDeLisa.ubi = (coor){31, 6};
				noviaDeLisa.HP = noviaDeLisa.HP - 25; //iba a chekear q tuviera mas de 0 hp pero directamente no podes salir a la calle con 0 hp
				return 1;
				break;

			// ENTRAR A UN VECINO
		    case 3:
		        if(noviaDeLisa.calleLoc-1 > noviaDeLisa.misionesCumplidas){
		            //cls();
		            printf("\n\tAVISO: Debes entregas la flor anterior.\n");
					freeze_ms(2000);
					printf("\e[%iA", 1);
		            printf("\t                                         \n");

		        }
		        else{
    		        if(noviaDeLisa.calleLoc <= noviaDeLisa.misionesCumplidas){
                        // noviaDeLisa.ubi.y = noviaDeLisa.ubi.y - 1; 
    	        	    printf("\nNOTA: Esta flor ya fue entregada!\n");
                		if (noviaDeLisa.lucides[noviaDeLisa.calleLoc-1] == 1){
							printf ("\t(alguien la entrego por ti)\n");
                		}
    	        	}
    		        else{
						// ENTRAS A LA CASA DEL VECINO
						return 3;
    	        	}
		        }
		        break;

			// CAMBIO DE CALLE
			case 4:
				return 2;
				break;
				
			default:
				break;
		}

		calleInput = getch();

		// MOVIMIENTO DE noviaDeLisa
		noviaDeLisa.ubi = movimientoConInput(calleInput, noviaDeLisa.ubi, escCalleLimit, 1);
		
	}while(estadoEnCalle != 6 && calleInput != '.');
}














int menuDeVecino(){

	coor auxUbiCalle = noviaDeLisa.ubi;

	int contador = (6 * noviaDeLisa.misionesCumplidas)+1;

	char vecinoInput;
	int vecinoContadorDeEscuchas = 0;

	int vecinoFocusOption = 1;
	char vecinoMensajeError[100] = "";

	int vecino2Escuchado = 0;
	int auxMisionesCumplidas = noviaDeLisa.misionesCumplidas;

	// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
	if(noviaDeLisa.cantFlores < 1){
		cinematica(35 + ((17*5) * auxMisionesCumplidas), 0);	//dibujo(casaDeVecino, 3);	
	}else{
		cinematica(52 + ((17*5) * auxMisionesCumplidas), 0);
	}

	actualizarMenuVecino(1);

	do{
		vecinoInput = getch();

		switch (vecinoInput)
		{
		case 'a':
		case 'A':
		case 'K':
			vecinoFocusOption = 1;
			actualizarMenuVecino(vecinoFocusOption);
			break;

		case 'd':
		case 'D':
		case 'M':
			vecinoFocusOption = 2;
			actualizarMenuVecino(vecinoFocusOption);
			break;

		case ' ':

			// Uso cls() porq asi es mas facil borrar el hud del vecino cuando comienza el minijuego.
			cls();
			marginTop();

			switch(vecinoFocusOption){
				case 1:
					// ESTE IF NO PERMITE ENTREGAR FLOR SIN FLOR EN MANO
					if(noviaDeLisa.cantFlores < 1) {
						strcpy(vecinoMensajeError, "\n\tNo llevas flores contigo ahora!!!");
						// printf("No llevas flores contigo ahora!!!");
						// freeze(3);
					} else{

						switch (vecinoGameplay()){

							// IMPACTASTE CON UN OSTACULO
							case 1:

								vecinoContadorDeEscuchas = vecinoContadorDeEscuchas + 1;
								contador = contador + 1;
								// noviaDeLisa.HP = noviaDeLisa.HP - 25;

								// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
								if(noviaDeLisa.cantFlores < 1){
									cinematica(35 + ((17*5) * auxMisionesCumplidas), 0);	//dibujo(casaDeVecino, 3);	
								}else{
									cinematica(52 + ((17*5) * auxMisionesCumplidas), 0);
								}

								actualizarMenuVecino(1);

								break;
							
								
							// PERDISTE TODO EL HP
							case 2:
								vecinoContadorDeEscuchas = vecinoContadorDeEscuchas + 1;
								
								// ESTE IF CHECKEA SI ESCUCHASTE TODO
								if (vecinoContadorDeEscuchas == 4){
									noviaDeLisa.lucides[noviaDeLisa.misionesCumplidas] = 1;

									cinematica(52 + ((17*5) * (auxMisionesCumplidas-1)), 3000);
									printf("\n\n");
									leerEscuchar(contador);
									// freeze(3);
								}

								noviaDeLisa.misionesCumplidas++;
								noviaDeLisa.cantFlores = 0;

								// reinicia ubi de noviaDeLisa cuando vuelve a casa
								noviaDeLisa.ubi = (coor){31, 6};
								// return para volver a casa
								return 1;
								break;

							// ENTREGASTE LA FLOR
							case 3:
								noviaDeLisa.misionesCumplidas++;
								noviaDeLisa.cantFlores = 0;

								cinematica(103, 2000);

								// Uso cls() porq asi es mas facil borrar el hud del vecino cuando volves a la casa.
								cls();
								marginTop();

								// reinicia ubi de noviaDeLisa cuando vuelve a casa
								noviaDeLisa.ubi = (coor){31, 6};	
								// return para volver a casa
								return 1;
								break;

							default:
								break;
						}

					}
					break;
				
				case 2:

					noviaDeLisa.ubi = auxUbiCalle;

					noviaDeLisa.ubi.y--;
					// printf("\e[%iA", 9);
					
					// Uso cls porq asi es mas facil borrar el hud del vecino cuando volves a la calle.
					cls();

					// VOLVES A LA CALLE
					return 2;
					break;
				
				default:
					// printf("\e[%iA", 100);
					break;
			}
			break;

		default:
			break;
		}

		// printf("\e[%iA", 5);

	//este while hay q cambiarlo porq no se va a poder guardar partida en un vecino
	}while(vecinoInput != '.');
}
















void* threadDibujo(void* arg) {
	int loc = *(int*)arg; // Convertir el argumento genérico a int
	// error1("loc: ");
	// error2(loc);
    while (seguirDibujando) {
        dibujarEscena(escVecino, loc);
		// error2(impactoConEntidad);

		// Asi entonces el thread se detiene antes de recibir el ultimo input.
		// Solo se detiene haciendo impacto con un "enemigo".
		if (impactoConEntidad == 1)
			pthread_exit(NULL);

        // usleep(100); // 100ms entre frames (ajustable)
		freeze_ms(fpsDibujo);
    }
    return NULL;
}

void* threadActualizarUbisDeObstaculos(void* arg) {
    while (seguirDibujando) {
		for (int m = 0; m < 5; m++){
			// todos los obstaculos se mueven 4 a la izq
			obstaculos[m].x = obstaculos[m].x - 4;

			// el obstaculo q se pasa d largo del mapa reinicia ubi
			if(obstaculos[m].x < 10)
				obstaculos[m] = (coor){56, noviaDeLisa.ubi.y};

		}
		freeze_ms(velocidadObstaculos);
    }
    return NULL;
}

void* threadActualizarUbisDeBalones(void* arg) {
    while (seguirDibujando) {

		// PRIMERO CHECKEA SI TE IMPACTA UN BALON
		// 	SINO ACTUALIZA LA UBI DEL BALON EN EL switch DE ABAJO
		for (int b = 0; b < 6; b++){

			// TODOS LOS if RESTARTEAN LA UBI DE LOS BALONES SI SE PASAN DE LARGO
			// 	SINO SOLO SE MUEVEN
			switch (b){
				case 0:
				case 5:
					if(balones[b].y < 4){
						balones[b].y = 14;
						balones[b].x = noviaDeLisa.ubi.x;
					} else{
						balones[b].y = balones[b].y - 1;
					}
					break;

				case 1:
				case 3:
					if(balones[b].x > 40){
						balones[b].x = 10;
						balones[b].y = noviaDeLisa.ubi.y;
					} else{
						balones[b].x = balones[b].x + 2;
					}
					break;

				case 2:
				case 4:
					if(balones[b].x < 20){
						balones[b].x = 50;
						balones[b].y = noviaDeLisa.ubi.y;
					} else{
						balones[b].x = balones[b].x - 2;
					}
					break;
				
				default:
					break;
			}
			
		}
		freeze_ms(velocidadBalones);
    }

    return NULL;
}
















int vecinoGameplay(){

	coor auxUbi = noviaDeLisa.ubi;
	int x = 1;
	char input;
	impactoConEntidad = 0;

	// marginTop();

	switch (noviaDeLisa.misionesCumplidas){
		case 0:
			cargaDeEscena(escVecino, 171);
			cargaDeEscena(escVecinoLimit, 188);

			noviaDeLisa.ubi = (coor){10, 7};
			actualizarHP(noviaDeLisa.HP);

			for (int i = 0; i < 5; i++) {
				obstaculos[i].x = 32 + (i*9);
				obstaculos[i].y = 11 - (i*2);
			}

			// INICIA EL MINIJUEGO DE LOS OBSTACULOS
			seguirDibujando = true;

			// Lanzar el thread de dibujo
			pthread_t hiloDibujo;
			int loc3 = 3;
			pthread_create(&hiloDibujo, NULL, threadDibujo, &loc3);

			// Lanzar el thread de actualizacion de ubis
			pthread_t hiloEntidades;
			pthread_create(&hiloEntidades, NULL, threadActualizarUbisDeObstaculos, NULL);

			char input = 0;
			while (input != '.' && impactoConEntidad == 0){
				noviaDeLisa.ubi = movimiento2(noviaDeLisa.ubi, escVecinoLimit, 3);
				// freeze_ms(100); // Pequeña pausa para evitar que el thread de dibujo consuma demasiados recursos
				// x = dibujarEscena(escVecino, 3); // opcional si querés forzar un frame tras el movimiento
			}

			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujo, NULL);
			pthread_join(hiloEntidades, NULL);

			noviaDeLisa.ubi = auxUbi;
			return impactoConEntidad;
			break;
		
		case 1:
			cargaDeEscena(escVecino, 205);
			cargaDeEscena(escVecinoLimit, 222);

			noviaDeLisa.ubi = (coor){30, 4};

			// PARA OCULTAR LOS BALONES CUANDO REINICIAR EL MINIJUEGO DESP DE UN IMPACTO
			for (int b = 1; b < 6; b++){
				balones[b].x = 100;
				balones[b].y = 100;
			}
			
			
			// x = dibujarEscena(escVecino, 4);
			
			// actualizarHP(noviaDeLisa.HP);
			
			balones[0] = (coor){30, 12};
			balones[5] = (coor){30, 18};
			balones[1] = (coor){15, noviaDeLisa.ubi.y};
			balones[2] = (coor){50, noviaDeLisa.ubi.y};
			balones[3] = (coor){-5, noviaDeLisa.ubi.y + 2};
			balones[4] = (coor){70, noviaDeLisa.ubi.y + 2};

			// ARREGLO CON LOS VECINO
			int los3Vecinos[3];
			for (int v = 0; v < 3; v++){
				los3Vecinos[v] = 0;
			}

			// INICIA EL MINIJUEGO DE LOS BALONES
			seguirDibujando = true;

			// Lanzar el thread de dibujo
			pthread_t hiloDibujo2;
			int loc = 4;
			pthread_create(&hiloDibujo2, NULL, threadDibujo, &loc);

			// Lanzar el thread de actualizacion de ubis
			pthread_t hiloEntidades2;
			pthread_create(&hiloEntidades2, NULL, threadActualizarUbisDeBalones, NULL);

			while (impactoConEntidad == 0 || impactoConEntidad == 3){
				// if (impactoConEntidad == 0){
					noviaDeLisa.ubi = movimiento2(noviaDeLisa.ubi, escVecinoLimit, 4);
				// } else {
				// 	impactoConEntidad = 0;
				// }
				
				
				// CHECKEA SI LLEGASTE A ALGUN VECINO
				if (noviaDeLisa.ubi.y == 12 || noviaDeLisa.ubi.x == 10 || noviaDeLisa.ubi.x == 50){
					// impactoConEntidad = 3;

						// CHECKEA A QUE VECINO LE ENTREGASTE LA FLOR
					switch (noviaDeLisa.ubi.x){
						case 30:
							noviaDeLisa.ubi.y = 10;

							// los3Vecinos[0] = 1;
							escVecino[2][30] = ' ';
							escVecino[2][31] = ' ';

							// memcpy(&escVecino[3][29], "     ", 5);
							escVecino[3][29] = ' ';
							escVecino[3][30] = ' ';
							escVecino[3][31] = ' ';
							escVecino[3][32] = ' ';
							escVecino[3][33] = ' ';

							escVecino[4][30] = ' ';
							escVecino[4][31] = ' ';

							escVecinoLimit[4][30] = '#';

							// movimientoConInput('s', noviaDeLisa.ubi, escVecinoLimit, 4);
							// movimientoConInput('s', noviaDeLisa.ubi, escVecinoLimit, 4);
							break;

						case 10:
							noviaDeLisa.ubi.x = 15;
							// los3Vecinos[1] = 1;
							escVecino[9][7] = ' ';
							escVecino[9][8] = ' ';
							
							escVecino[10][6] = ' ';
							escVecino[10][7] = ' ';
							escVecino[10][8] = ' ';
							escVecino[10][9] = ' ';

							escVecino[11][7] = ' ';
							escVecino[11][8] = ' ';

							escVecinoLimit[10][10] = '#';

							// movimientoConInput('d', noviaDeLisa.ubi, escVecinoLimit, 4);
							error1("izquierda");
							break;

						case 50:
							noviaDeLisa.ubi.x = 45;

							// los3Vecinos[2] = 1;
							escVecino[9][53] = ' ';
							escVecino[9][54] = ' ';
															
							escVecino[10][51] = ' ';
							escVecino[10][52] = ' ';
							escVecino[10][53] = ' ';
							escVecino[10][54] = ' ';
							escVecino[10][55] = ' ';

							escVecino[11][53] = ' ';
							escVecino[11][54] = ' ';

							escVecinoLimit[10][50] = '#';

							break;
						
						default:
							break;
					}						
						
					// SI ENTREGASTE LAS 3 FLORES EL MINIJUEGO TERMINA
					// if ((los3Vecinos[0] + los3Vecinos[1] + los3Vecinos[2]) == 3){
					// 	cinematica(188, 2000);
					// 	return 3;
					// }

					// POR SI QUIERO QUE NO SE MUEVAN LOS BALONES JUSTO DESP DE ENTREGAR UNA FLOR
					// 	PERO ESO HARIA MUY FACIL EL MINIJUEGO
					// balones[0].y = balones[0].y + 2;
					// balones[5].y = balones[5].y + 2;
					// balones[1].x = balones[1].x - 5;
					// balones[3].x = balones[3].x - 5;
					// balones[2].x = balones[2].x + 5;
					// balones[4].x = balones[4].x + 5;

					// x = dibujarEscena(escVecino, 4);
				}
			
				// x = dibujarEscena(escVecino, 4);
			}				

			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujo2, NULL);
			pthread_join(hiloEntidades2, NULL);

			noviaDeLisa.ubi = auxUbi;

			return x;
			break;

		default:
			break;
	}
}















#define CABEZA "--"
#define CUERPO "./\\."
#define PIES "!!"
#define FLOR "*"

// ESTA FUNCION DIBUJA UN FOTOGRAMA x INPUT Y DEVUELVE UN NRO DEPENDIENDO CON QUE HACES CONTACTO.
// 		SI NO HACES CONTACTO CON NADA DEVUELVE 0
int dibujarEscena(char d[max][max2], int loc){

	// marginTop();
	printf("\e[%iA", 15);

    switch(loc){
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
        case 1:		

			// 		ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = max-1; i > 0; i--){
                printf("                              ");
                for(int j = 1; j < max2; j++){
					if(i == noviaDeLisa.ubi.y+2 && j == noviaDeLisa.ubi.x){
						printf(CABEZA);
						j++;

					}else if(i == noviaDeLisa.ubi.y+1 && j == noviaDeLisa.ubi.x-1){
                        printf(CUERPO);
						if(noviaDeLisa.cantFlores > 0){
							printf(FLOR);
							j++;
						}
                        j = j + 3;

                    }else if(i == noviaDeLisa.ubi.y && j == noviaDeLisa.ubi.x){
                        printf(PIES);
						j++;

                    }else{
                        printf("%c", d[max-i][j]);
                    }
                }
                printf("\n");
            }

			// 		ESTOS IF CHEKEAN SI HACES CONTACTO CON ALGO
			//  (algunos if es mejor ponerlos desp de dibujar la escena)

			// SALISTE A LA CALLE POR LA PUERTA
			if(proximo(noviaDeLisa.ubi, puertaACalle) == 1)
				return 1;

			// AGARRASTE UNA FLOR
			if(proximo(noviaDeLisa.ubi, flor) == 1 && noviaDeLisa.cantFlores == 0)
				return 2;

			// COMISTE LOS CARAMELOS
            if(proximo(noviaDeLisa.ubi, curas) == 1 && noviaDeLisa.HP < 100)
				return 3;

            break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 2:											//DIBUJA LA ESCENA DE LA CALLE

			// 	ESTO ES PARA PASAR DE CALLE
			// 		AVANZAR DE CALLE
			if(noviaDeLisa.ubi.x > 59){
				noviaDeLisa.calleLoc++;
				// if(noviaDeLisa.calleLoc <= noviaDeLisa.misionesCumplidas + 1){		//todo lo comentado funciona, esta comentado para poder testear cosas en el mapa
					noviaDeLisa.ubi.x = 1;
					return 4;
				// } else {
				// 	noviaDeLisa.ubi.x = noviaDeLisa.ubi.x - 2;
				// 	noviaDeLisa.calleLoc--;
				// }
			}
            
			// 		VOLVER UNA CALLE
			if(noviaDeLisa.ubi.x < 1){
				noviaDeLisa.calleLoc--;
				// if (noviaDeLisa.calleLoc >= 1){		//todo lo comentado funciona, esta comentado para poder testear cosas en el mapa
					noviaDeLisa.ubi.x = 59;
					return 4;
				// } else {
				// 	noviaDeLisa.ubi.x = noviaDeLisa.ubi.x + 2;
				// 	noviaDeLisa.calleLoc++;
				// }
			}

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = max-1; i > 0; i--){
                printf("                              ");
                for(int j = 1; j < max2; j++){
					
                    if((noviaDeLisa.calleLoc == noviaDeLisa.misionesCumplidas) && (noviaDeLisa.misionesCumplidas > 0) && ((j == entyCoor[1].x) && (i == entyCoor[1].y) || (j == entyCoor[2].x) && (i == entyCoor[2].y) || (j == entyCoor[3].x) && (i == entyCoor[3].y))){
                        printf("GB-");
                        j = j + 2;
                    }else if(i == noviaDeLisa.ubi.y && j == noviaDeLisa.ubi.x){
                        printf("L");
						if(noviaDeLisa.cantFlores > 0){
							printf("*"); 
							j++;
						}
                    } else{
                        printf("%c", d[max-i][j]);
                    }
                }
				// El ' ' en el printf es para borrar la flor q se buguea a la derecha de la escena de la calle.
                printf(" \n");
            }

			// 		ESTOS IF CHEKEAN SI HACES CONTACTO CON ALGO
			// ENTRAR A CASA
            if(noviaDeLisa.calleLoc==1 && proximo(noviaDeLisa.ubi, entyCoor[10])==2)
				return 1;

			// ATRAPADA POR ABEJA
				// habria que averiguar si ya con checkear el (noviaDeLisa.calleLoc == noviaDeLisa.misionesCumplidas) == false
				// 		salta de linea o hace todos los checkeos del if, por temas de rendimiento
            if((noviaDeLisa.calleLoc == noviaDeLisa.misionesCumplidas) && (noviaDeLisa.cantFlores > 0 && noviaDeLisa.misionesCumplidas > 0) && ((proximo(noviaDeLisa.ubi, entyCoor[1]) > 0) || (proximo(noviaDeLisa.ubi, entyCoor[2]) > 0) || (proximo(noviaDeLisa.ubi, entyCoor[3]) > 0)))
                return 2;	

			// ENTRAR A VECINO 1, 2, 3 o 4
			if(proximo(noviaDeLisa.ubi, entyCoor[10 + noviaDeLisa.calleLoc]) == 2)
				return 3;	

            break;
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 3:											//DIBUJA LA ESCENA DEL VECINO 1

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = max-1; i > 0; i--){
                printf("                              ");
                for(int j = 1; j < max2; j++){

					if((j == obstaculos[0].x) && (i == obstaculos[0].y)
						||
						(j == obstaculos[1].x) && (i == obstaculos[1].y)
						||
						(j == obstaculos[2].x) && (i == obstaculos[2].y)
						||
						(j == obstaculos[3].x) && (i == obstaculos[3].y)
						||
						(j == obstaculos[4].x) && (i == obstaculos[4].y)
					){
                        // printf("_[C");
                        printf("@@@");
						
						// if((j == noviaDeLisa.ubi.x-1) && (i == noviaDeLisa.ubi.y))
						if(proximo(noviaDeLisa.ubi, (coor){j+1, i}) > 0)
							impactoConEntidad = 1;

						j = j+2;

                    }else if((j == noviaDeLisa.ubi.x) && (i == noviaDeLisa.ubi.y) && impactoConEntidad == 0){
                        printf("L*");
						j++;
                    } else{
                        printf("%c", d[max-i][j]);
                    }
                }
				// este prntf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf("               ");
                printf("\n");
            }

			// CHECKEA SI LLEGASTE AL VECINO
			if (noviaDeLisa.ubi.x > 50 && noviaDeLisa.ubi.y < 10 && noviaDeLisa.ubi.y > 4){
				// cinematica(103);
				// freeze(2);
				impactoConEntidad = 3;
				return 3;
			}

			break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 4:											//DIBUJA LA ESCENA DEL VECINO 2

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = max-1; i > 0; i--){
                printf("                              ");
                for(int j = 1; j < max2; j++){

					if (((j == balones[0].x+1) && (i == balones[0].y+1)) || ((j == balones[5].x) && (i == balones[5].y+1))){
						printf("s");

					}else if(((j == balones[0].x) && (i == balones[0].y)) || ((j == balones[5].x) && (i == balones[5].y))){
                        printf("()");
						
						if (proximo(noviaDeLisa.ubi, (coor){j, i}) >= 2){
							noviaDeLisa.HP = noviaDeLisa.HP - 25;
							impactoConEntidad = 1;
						}
						j = j+1;
						
                    }else if (((j == balones[1].x) && (i == balones[1].y)) || ((j == balones[3].x) && (i == balones[3].y))){
						printf("~O)");
						
						if (proximo(noviaDeLisa.ubi, (coor){j+1, i}) >= 1){
							noviaDeLisa.HP = noviaDeLisa.HP - 25;
							impactoConEntidad = 1;
						}
						j = j+2;
						
					}else if(((j == balones[2].x-1) && (i == balones[2].y)) || ((j == balones[4].x-1) && (i == balones[4].y))){
						printf("(O~");
						
						if (proximo(noviaDeLisa.ubi, (coor){j, i}) >= 1){
							noviaDeLisa.HP = noviaDeLisa.HP - 25;
							impactoConEntidad = 1;
						}
						j = j+2;
						
					}else if((j == noviaDeLisa.ubi.x) && (i == noviaDeLisa.ubi.y)){
                        printf("L*");
						j++;
						
                    } else{
                        printf("%c", d[max-i][j]);
                    }
                }
				// este prntf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf("               ");
                printf("\n");
            }

			break;

        default:
            break;
    }
    return 0;
}