#include "auxFuncs.h"
#include "cinematicas.h"

// noviaDeLisa es el/la personaje jugable
Player noviaDeLisa;

// Lista de coordenadas de algunas entidades
coor entyCoor[20];

coor flor;
coor puertaACalle;
coor curas;


// Matrices que guardan las escenas
typedef struct {
	char escVisual[maxV][maxH];
	char escVisualAlt[maxV][maxH];
	char escLimit[maxV][maxH];
} escCollection;
escCollection listaDeEscenas[10];


// FUNCIONES PRINCIPALES (y sus funciones particulares)
int menuDeCasa();

int menuDeCalle();
	void consultarImpactoConAbeja();

int menuDeVecino();
	int vecinoGameplay();

// 	Mas funciones
int dibujarEscena(char d[maxV][maxH], int loc);


//lo uso para cargar la data guardada en el pj
//	deberia estar en el main pero para eso tengo q arreglar el tema de los punteros
char saveArray[18];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES NECESARIAS PRINCIPALMENTE PARA LOS HILOS.

float fps = 20;							// Frames por segundo de los minijuegos.
float fpsDibujo;						// Milisegundos entre cada frame de dibujo (calculado a partir de fps).
volatile int impactoConEntidad = 0;		// Representa si hubo impacto/contacto con algun obstaculo, balon, vecino, etc. 
										// 	para detener los hilos.
volatile bool seguirDibujando = true;	// Variable para controlar el bucle de dibujo en los threads

//	Utilizadas solo en menuCalle()
	volatile bool timeChange = true;
	
// 	Utilizadas solo en vecinoGameplay()
	// Obstaculos del juego de 1er vecino
	coor obstaculos[5];
	int velocidadObstaculos = 200;
	
	// Balones del juego de 2do vecino
	coor balones[6];
	int velocidadBalones = 200;
	
	// Cosas del juego de 3er vecino
	volatile int timeSwitch = 0;
	volatile int olaUbi = 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



















// ESTA FUNCION TE VA A MOVER ENTRE DIFERENTES MENUS JUGABLES (CASA, CALLE, CASA DE VECINO)
void startGame(Player *actualGame){

		// Este if funciona suponiendo qie nunca voy a poder alcanzar la ubi y=0
		// 	Por eso creo que tendria q cambiarlo.
	// Este if mantiene guardada la partida actual cuando vos volves al menu principal.
	if (actualGame->ubi.y != 0){
		noviaDeLisa = *actualGame;
		
		//	SI EXISTE EL ARCHIVO save.txt ENTONCES CARGA LOS DATOS EN EL PERSONAJE
		//		SINO INICIALIZA EL PERSONAJE DESDE 0
	} else if (access("save.txt", F_OK) == 0) {
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
			/**/ noviaDeLisa.hp = atoi(&saveArray[7]) / 10000000;
			/**/
			/**/ noviaDeLisa.ubi.x = atoi(&saveArray[10]) / 100000;
			/**/ noviaDeLisa.ubi.y = atoi(&saveArray[12]) / 1000;	
			/**/
			/**/ cAux = saveArray[14];
			/**/ noviaDeLisa.calleLoc = atoi(&cAux);
			/**/
			/**/ noviaDeLisa.colorL = atoi(&saveArray[15]);
			////////////////////////////////////////////////////////////////
		}

		fclose(saveFile);
		
	} else {
		
		/////////////////INICIALISACION DE PESONAJE/////////////////////
		/**/ noviaDeLisa.hp = 80;
		/**/ noviaDeLisa.cantFlores = 0;
		/**/ noviaDeLisa.misionesCumplidas = 0;
		/**/ noviaDeLisa.primeraVez = 0;
		/**/ for(int ini = 0; ini<=3; ini++){
		/**/ 	noviaDeLisa.lucides[ini] = 0;
		/**/ }
		/**/ noviaDeLisa.ubi = (coor){31, 7};	//noviaDeLisa por primera vez en el juego
		/**/ noviaDeLisa.calleLoc = 0;
		/**/ noviaDeLisa.colorL = 37;
		////////////////////////////////////////////////////////////////
		
		guardarPartida(&noviaDeLisa, 0); // Guardo la partida inicial automaticamente
	}

	/////////////////INICIALISACION DE ENTIDADES/////////////////////
	/**/ flor = (coor){46, 11};
	/**/ puertaACalle = (coor){56, 7};
	/**/ curas = (coor){24, 5};
	/**/
	/**/ entyCoor[1] = (coor){30, 8};		//abeja 1
	/**/ entyCoor[2] = (coor){18, 11};		//abeja 2
	/**/ entyCoor[3] = (coor){45, 5};		//abeja 3
	/**/ entyCoor[10] = (coor){9, 7};		//puerta para entrar a casa
	/**/ entyCoor[11] = (coor){35, 14};		//puerta para entrar a vecino 1
	/**/ entyCoor[12] = (coor){31, 13};		//puerta para entrar a vecino 2
	/**/ entyCoor[13] = (coor){41, 13};		//puerta para entrar a vecino 3
	/**/ entyCoor[14] = (coor){31, 12};		//puerta para entrar a vecino 4
	////////////////////////////////////////////////////////////////

	/////////////////CARGA INICIAL DE ESCENAS/////////////////////
	/*Escena de casa*/		cargarEscenasConAlternativa(listaDeEscenas[0].escVisual, listaDeEscenas[0].escVisualAlt, listaDeEscenas[0].escLimit, 1);		
	/*Escena de calle 1*/	cargarEscenasConAlternativa(listaDeEscenas[1].escVisual, listaDeEscenas[1].escVisualAlt, listaDeEscenas[1].escLimit, 52);		
	/*Escena de calle 2*/	cargarEscenasConAlternativa(listaDeEscenas[2].escVisual, listaDeEscenas[2].escVisualAlt, listaDeEscenas[2].escLimit, 103);		
	/*Escena de calle 3*/	cargarEscenasConAlternativa(listaDeEscenas[3].escVisual, listaDeEscenas[3].escVisualAlt, listaDeEscenas[3].escLimit, 154);		
	/*Escena de calle 4*/	cargarEscenasConAlternativa(listaDeEscenas[4].escVisual, listaDeEscenas[4].escVisualAlt, listaDeEscenas[4].escLimit, 205);		
	/**/
	/**///Las escenas de los vecino se cargan en vecinoGameplay() porq se tienen que reiniciar por cada impacto recibido
	/**///listaDeEscenas[5] = vecino 1
	/**///listaDeEscenas[6] = vecino 2
	/**///listaDeEscenas[7] = vecino 3
	/**///listaDeEscenas[8] = vecino 4
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
		// Cada que entro a un nuevo menu dejo el cursor en la ubicasion correcta.
		ubicarPivote();

		// CADA MENU DEVUELVE UN NRO QUE INDICA A Q NUEVA ESCENA/MENU VA EL PERSONAJE
		switch(op){
			// menuDeCasa
			case 1:
				op = menuDeCasa();
				break;
			
			// menuDeCalle
			case 2:
				op = menuDeCalle();
				break;
			
			// menuDeVecino
			case 3:
				op = menuDeVecino();
				break;
			

			// Vovles al menu.
			case 6:
				// Cuando vuelvas al menu principal guardas la partida actual temporalmente
				*actualGame = noviaDeLisa;
				return;
                break;

			// default:
			// 	// Cuando vuelvas al menu principal guardas la partida actual temporalmente
			// 	*actualGame = noviaDeLisa;
            //     op = 7;
            //     break;
		}
	}while(op != 7);
}

















	// Ya no lo necesito pero lo guardo por las dudas.
    //system ("/bin/stty raw"); //FUNCIONA PARA Q GETCHAR() FUNCIONE EN UBUNTU

int menuDeCasa(){
	noviaDeLisa.calleLoc = 0;
	actualizarHP(noviaDeLisa.hp);
	int estadoEnCasa = 0;
	char casaInput = 'x';

    while(casaInput != '.' && casaInput != '\033'){
    // while(estadoEnCasa == 0){

		//	DEVUELVE 1 SI SALISTE A LA CALLE
		//	SINO DEVUELVE 0
        estadoEnCasa = dibujarEscena(listaDeEscenas[0].escVisual, 1);

		// debugNdlData(&noviaDeLisa);
        
        switch(estadoEnCasa){
            case 0:
                // casaInput = getch();
                break;

			// SALIO A LA CALLE POR LA PUERTA.
            case 1:
				// ESTE IF NO TE DEJA SALIR DE CASA CON 0 DE ENERGIA.
				if(noviaDeLisa.hp <= 0) {
					noviaDeLisa.ubi.x = noviaDeLisa.ubi.x - 3;
			        estadoEnCasa = dibujarEscena(listaDeEscenas[0].escVisual, 1);
				} else {
					noviaDeLisa.ubi = (coor){11, 7};
					noviaDeLisa.calleLoc = 1;
					return 2;
				}
                break;
				
			// AGARRASTE UNA FLOR.
			case 2:
				noviaDeLisa.cantFlores = 1;
				
				agarrarFlorCinematica();

				estadoEnCasa = dibujarEscena(listaDeEscenas[0].escVisual, 1);
				actualizarHP(noviaDeLisa.hp);
				break;

			// AGARRASTE UN CARAMELO.
			case 3:
				tomarCurasCinematica();

				actualizarHP(noviaDeLisa.hp);
				sleep_ms(500);
				do{
					sleep_ms(100);
					noviaDeLisa.hp = noviaDeLisa.hp + 10;
					actualizarHP(noviaDeLisa.hp);
				}while (noviaDeLisa.hp < 80);
				sleep_ms(700);

				estadoEnCasa = dibujarEscena(listaDeEscenas[0].escVisual, 1);
				break;

            default:
                break;
        }

		casaInput = getch();

		if (casaInput == '.' || casaInput == '\033')
			return 6;		
        
		// MOVIMIENTO DE noviaDeLisa
		noviaDeLisa.ubi = movimientoConInput(casaInput, noviaDeLisa.ubi, listaDeEscenas[0].escLimit, 10);
    }
	return 6;		
}















void* threadDibujoCalle(void* arg);
void* threadTimeChange();
void* threadActualizarCoorAbejas();

int menuDeCalle (){

	// Con esto oculto la barra de hp en la calle.
    printf("\n\n\t                                                                      \n");
    printf("\e[%iA", 3);
	
	char calleInput;
	
	// Lanzar el thread de dibujo
	seguirDibujando = true;
	impactoConEntidad = 0;

	int loc2 = 2;
	pthread_t hiloDibujoCalle;
	pthread_t hiloTimeChange;
	pthread_t hiloAbejas;

	pthread_create(&hiloDibujoCalle, NULL, threadDibujoCalle, &loc2);
	
	// Si no deberia haber abejas inicia solo el hilo del timeChange
	// 	así tengo solo dos hilos en ejecucion en la calle
	if (noviaDeLisa.calleLoc != noviaDeLisa.misionesCumplidas){
		pthread_create(&hiloTimeChange, NULL, threadTimeChange, &loc2);

	// Sino inicia el thread de las abejas
	// 	(este hilo tambien modifica el timeChange)
	} else {
		pthread_create(&hiloAbejas, NULL, threadActualizarCoorAbejas, &loc2);

		//	REINICIA UBI DE ABEJAS CADA QUE LLEGAS A LA CALLE
		entyCoor[1] = (coor){30, 8};	//abeja 1
		entyCoor[2] = (coor){18, 11};	//abeja 2
		entyCoor[3] = (coor){45, 5};	//abeja 3
		// Podria hacer q se guarden las ubis en el actualGame para q no se reinicien las ubis cuando abris el menu
		// 	o no permitir abrir el menu en la calle con abejas
		// 	Sin duda quedaria mejor guardar las ubis pero seria mucho trabajo cargarlas a cada rato
	}

	// Mientras no impactes con nada va a seguir esperando inputs
	while(impactoConEntidad == 0){

		sleep_ms(50);
		calleInput = getch();

		if (calleInput == '.' || calleInput == '\033'){
			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujoCalle, NULL);
			pthread_join(hiloTimeChange, NULL);
			pthread_join(hiloAbejas, NULL);
	
			return 6;		
		}

		// MOVIMIENTO DE noviaDeLisa
		noviaDeLisa.ubi = movimientoConInput(calleInput, noviaDeLisa.ubi, listaDeEscenas[noviaDeLisa.calleLoc].escLimit, 0);

		consultarImpactoConAbeja();

		// Algunos impactos no es necesario hacerlos por fotograma y los hago por input.
		//	ENTRAR A CASA
		if(noviaDeLisa.calleLoc==1 && proximo(noviaDeLisa.ubi, entyCoor[10])==2){
			impactoConEntidad = 1;
			sleep_ms(100);

			noviaDeLisa.ubi = (coor){52, 7};
		}

		// ENTRAR A VECINO 1, 2, 3 o 4
		if(proximo(noviaDeLisa.ubi, entyCoor[10 + noviaDeLisa.calleLoc]) == 2){
			dibujarEscena(listaDeEscenas[noviaDeLisa.calleLoc].escVisual, 2);
			if(noviaDeLisa.calleLoc-1 > noviaDeLisa.misionesCumplidas){
				impactoConEntidad = 2;
				sleep_ms(100);
				
				printf("\e[%iA", 1);
				printf("\tDebes entregas la flor anterior.\n");
				sleep_ms(1000);
				printf("\e[%iA", 1);
				printf("\t                                         \n");

			} else if(noviaDeLisa.calleLoc <= noviaDeLisa.misionesCumplidas){
				impactoConEntidad = 2;
				sleep_ms(100);

				printf("\e[%iA", 6);
				printf("\t\t\t\t\t	 _______________________________ \n");
				printf("\t\t\t\t\t	|                               |\n");
				printf("\t\t\t\t\t	|  Esta flor ya fue entregada!  |\n");
				printf("\t\t\t\t\t	|_______________________________|\n");
				ubicarPivote();
				noviaDeLisa.ubi.y--;
				sleep_ms(1500);

				if (noviaDeLisa.lucides[noviaDeLisa.calleLoc-1] == 1){
					printf ("\t(alguien la entrego por ti)\n");
					printf("\e[%iA", 1);
				}

			} else{
				// ENTRAS A LA CASA DEL VECINO
				impactoConEntidad = 3;
				sleep_ms(100);
			}
		}

		//	AVANZAR DE CALLE
		if(noviaDeLisa.ubi.x > 59){
			impactoConEntidad = 2;
			sleep_ms(100);

			cambiarCalleAnim(listaDeEscenas[noviaDeLisa.calleLoc].escVisual, listaDeEscenas[noviaDeLisa.calleLoc+1].escVisual, 'd');
			noviaDeLisa.calleLoc++;
			noviaDeLisa.ubi.x = 1;
		}
		
		//	VOLVER UNA CALLE
		if(noviaDeLisa.ubi.x < 1){
			impactoConEntidad = 2;
			sleep_ms(100);

			cambiarCalleAnim(listaDeEscenas[noviaDeLisa.calleLoc].escVisual, listaDeEscenas[noviaDeLisa.calleLoc-1].escVisual, 'i');
			noviaDeLisa.calleLoc--;
			noviaDeLisa.ubi.x = 59;
		}
	};

	// Una vez hiciste impacto con algo detiene los threads
	seguirDibujando = false;
	pthread_join(hiloDibujoCalle, NULL);
	pthread_join(hiloTimeChange, NULL);
	pthread_join(hiloAbejas, NULL);

	return impactoConEntidad;
}

void* threadDibujoCalle(void* arg) {
	int loc = *(int*)arg; // Convertir el argumento genérico a int
    while (seguirDibujando) {

		if (timeChange){
			dibujarEscena(listaDeEscenas[noviaDeLisa.calleLoc].escVisual, loc);
		} else {
			dibujarEscena(listaDeEscenas[noviaDeLisa.calleLoc].escVisualAlt, loc);
		}
		
		// Asi entonces el thread se detiene antes de recibir el ultimo input.
		// Solo se detiene haciendo impacto con un "enemigo".
		if (impactoConEntidad != 0)
			pthread_exit(NULL);

		sleep_ms(40);
    }
    return NULL;
}

void* threadTimeChange() {
    while (seguirDibujando) {

		sleep_ms(1000);
		timeChange = !timeChange;

		if (impactoConEntidad != 0)
			pthread_exit(NULL);
    }
    return NULL;
}

void* threadActualizarCoorAbejas() {
    while (seguirDibujando) {
		entyCoor[1] = randomUbi(entyCoor[1], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		entyCoor[2] = randomUbi(entyCoor[2], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		entyCoor[3] = randomUbi(entyCoor[3], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		
		consultarImpactoConAbeja();

		if (impactoConEntidad != 0){
			pthread_exit(NULL);
		}

		sleep_ms(500);

	/////////////////////////////////////
		entyCoor[1] = randomUbi(entyCoor[1], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		entyCoor[2] = randomUbi(entyCoor[2], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		entyCoor[3] = randomUbi(entyCoor[3], listaDeEscenas[noviaDeLisa.calleLoc].escLimit);
		
		consultarImpactoConAbeja();

		if (impactoConEntidad != 0){
			pthread_exit(NULL);
		}

		sleep_ms(500);

		timeChange = !timeChange;
    }
    return NULL;
}

// Sirve para consultar el impacto con las abejas 
// 	y solo lo uso cuando se mueve una abeja o ndl (y no por fps)
void consultarImpactoConAbeja(){

		if((noviaDeLisa.cantFlores > 0) && 
			noviaDeLisa.calleLoc == noviaDeLisa.misionesCumplidas && (
			(proximoAbeja(noviaDeLisa.ubi, entyCoor[1]) > 0) || 
			(proximoAbeja(noviaDeLisa.ubi, entyCoor[2]) > 0) || 
			(proximoAbeja(noviaDeLisa.ubi, entyCoor[3]) > 0)
		)){
			impactoConEntidad = 1;
			sleep_ms(1000);
			// debugStringCoor("ndl", noviaDeLisa.ubi);
			// debugStringCoor("abeja 1", entyCoor[1]);
			// debugStringCoor("abeja 2", entyCoor[2]);
			// debugStringCoor("abeja 3", entyCoor[3]);

			ubicarPivote();
			cinematica(18, 3000);

			noviaDeLisa.cantFlores = 0;
			noviaDeLisa.ubi = (coor){31, 7};

			// hp a veces puede ser 10
			if (noviaDeLisa.hp >= 20){
				noviaDeLisa.hp = noviaDeLisa.hp - 20; //iba a chekear q tuviera mas de 0 hp pero directamente no podes salir a la calle con 0 hp
			} else {
				noviaDeLisa.hp = 0;
			}
		}

}














int menuDeVecino(){
	cls();
	ubicarPivote();

	coor auxUbiCalle = noviaDeLisa.ubi;
	int auxMisionesCumplidas = noviaDeLisa.misionesCumplidas;
	int vecinoFocusOption = 1;

	// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
	if(noviaDeLisa.cantFlores < 1){
		cinematica(35 + ((17*5) * auxMisionesCumplidas), 0);	//dibujo(casaDeVecino, 3);	
	}else{
		cinematica(52 + ((17*5) * auxMisionesCumplidas), 0);
	}

	actualizarMenuVecino(1);

	do{
		switch (getch()){
			case 'a':
			case 'A':
			case 75:
				vecinoFocusOption = 1;
				actualizarMenuVecino(1);
				break;

			case 'd':
			case 'D':
			case 77:
				vecinoFocusOption = 2;
				actualizarMenuVecino(2);
				break;

			case ' ':
			case '\r':
				// Uso cls() porq asi es mas facil borrar el hud del vecino cuando comienza el minijuego.
				cls();
				ubicarPivote();

				switch(vecinoFocusOption){
					
					//	VOLVES A LA CALLE
					case 2:
						noviaDeLisa.ubi = auxUbiCalle;
						noviaDeLisa.ubi.y--;
						
						// Uso cls porq asi es mas facil borrar el hud del vecino cuando volves a la calle.
						cls();

						return 2;
						break;

					//	COMIENZA EL MINIJUEGO
					case 1:
						// ESTE IF NO PERMITE ENTREGAR FLOR SIN FLOR EN MANO
						if(noviaDeLisa.cantFlores < 1) {
							cinematica(35, 0);
							printf("\e[%iA", 1);
							printf("\tNo llevas flores contigo ahora!!!\n");
							actualizarMenuVecino(vecinoFocusOption);

						} else{
							switch (vecinoGameplay()){

								// IMPACTASTE CON UN OSTACULO
								case 1:

									// Esto funciona suponiendo que todos los obstaculos de todos los minijuegos van a sacar 10hp
									noviaDeLisa.hp = noviaDeLisa.hp - 10;

									// Si todavia tenes hp seguis en la casa del vecino
									if (noviaDeLisa.hp > 0){

										// PRIMERA ESCENA EN VECINO, CON O SIN FLOR
										if(noviaDeLisa.cantFlores < 1){
											cinematica(35 + ((17*5) * auxMisionesCumplidas), 0);	//dibujo(casaDeVecino, 3);	
										}else{
											cinematica(52 + ((17*5) * auxMisionesCumplidas), 0);
										}
		
										actualizarMenuVecino(1);

									// Pero si perdes todo el hp volves a casa
									} else {
										noviaDeLisa.misionesCumplidas++;
										noviaDeLisa.cantFlores = 0;

										// reinicia ubi de noviaDeLisa cuando vuelve a casa
										noviaDeLisa.ubi = (coor){31, 7};

										// return para volver a casa
										return 1;
									}
									break;

								// ENTREGASTE LA FLOR
								case 2:
									noviaDeLisa.misionesCumplidas++;
									noviaDeLisa.cantFlores = 0;

									// Uso cls() porq asi es mas facil borrar el hud del vecino cuando volves a la casa.
									cls();
									ubicarPivote();

									// reinicia ubi de noviaDeLisa cuando vuelve a casa
									noviaDeLisa.ubi = (coor){31, 7};

									// return para volver a casa
									return 1;
									break;

								default:
									break;
							}

						}
						break;
					
					default:
						break;
				}
				break;

			default:
				break;
		}

	//deberia cambiar la condicion
	}while(vecinoFocusOption != 10);
}	

















void* threadDibujoVecino(void* arg);
void* threadActualizarUbisDeObstaculos(void* arg);
void* threadActualizarUbisDeBalones(void* arg);
void* threadTimeSwitch();

// vecinoGameplay() devuelve 2 posibles estados:
// 	impactoConEntidad = 1 --> impacto con obstaculo / recibir daño
// 	impactoConEntidad = 2 --> entregaste la flor
// 	no puede devolver 0 porq ese estado de impactoConEntidad significa que no hizo contacto con nada
int vecinoGameplay(){

	fpsDibujo = 1000 / fps; 
	coor auxUbi = noviaDeLisa.ubi;
	char input;
	impactoConEntidad = 0;

	switch (noviaDeLisa.misionesCumplidas+1){
		// PRIMER VECINO - MINIJUEGO DE LOS OBSTACULOS
		case 1:
			noviaDeLisa.ubi = (coor){9, 8};
			actualizarHP(noviaDeLisa.hp);
			cargarEscenas(listaDeEscenas[5].escVisual, listaDeEscenas[5].escLimit, 256);

			for (int i = 0; i < 5; i++) {
				obstaculos[i].x = 36 + (i*9);
				obstaculos[i].y = 12 - (i*2);
			}

			// THREADS
			seguirDibujando = true;
			impactoConEntidad = 0;
			pthread_t hiloDibujo;
			pthread_t hiloEntidades;
			int loc3 = 3;
			pthread_create(&hiloDibujo, NULL, threadDibujoVecino, &loc3);
			pthread_create(&hiloEntidades, NULL, threadActualizarUbisDeObstaculos, NULL);

			while (impactoConEntidad == 0){

				noviaDeLisa.ubi = movimientoConInput(getch(), noviaDeLisa.ubi, listaDeEscenas[5].escLimit, 1);

				// CHECKEA SI LLEGASTE AL VECINO
				if (noviaDeLisa.ubi.x > 50){
					impactoConEntidad = 2;
					sleep_ms(100);
					cinematica(103, 2000);
				}
			}			

			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujo, NULL);
			pthread_join(hiloEntidades, NULL);

			noviaDeLisa.ubi = auxUbi;

			return impactoConEntidad;
			break;
		
		// SEGUNDO VECINO - MINIJUEGO DE LOS BALONES
		case 2:
			noviaDeLisa.ubi = (coor){30, 5};
			actualizarHP(noviaDeLisa.hp);
			cargarEscenas(listaDeEscenas[6].escVisual, listaDeEscenas[6].escLimit, 290);
			int cuantosVecinosRecibieronFlor = 0;
			
			// INICIALIZA LAS UBIS DE LOS BALONES
			// Estos balones se mueven hacia abajo
			balones[0] = (coor){30, 13};
			balones[5] = (coor){noviaDeLisa.ubi.x, 19};

			// Estos balones se mueven a la derecha
			balones[1] = (coor){20, noviaDeLisa.ubi.y + 2};
			balones[3] = (coor){0, noviaDeLisa.ubi.y};

			// Estos balones se mueven a la izquierda
			balones[2] = (coor){50, noviaDeLisa.ubi.y};
			balones[4] = (coor){70, noviaDeLisa.ubi.y + 2};

			// THREADS
			seguirDibujando = true;
			impactoConEntidad = 0;
			pthread_t hiloDibujo2;
			pthread_t hiloEntidades2;
			int loc = 4;
			pthread_create(&hiloDibujo2, NULL, threadDibujoVecino, &loc);
			pthread_create(&hiloEntidades2, NULL, threadActualizarUbisDeBalones, NULL);

			while (impactoConEntidad == 0){

				noviaDeLisa.ubi = movimientoConInput(getch(), noviaDeLisa.ubi, listaDeEscenas[6].escLimit, 2);
				
				// CHECKEA SI LLEGASTE A ALGUN VECINO DESP DE CADA INPUT
				if (noviaDeLisa.ubi.y == 13 || noviaDeLisa.ubi.x == 10 || noviaDeLisa.ubi.x == 50){

					// CHECKEA A QUE VECINO LE ENTREGASTE LA FLOR
					switch (noviaDeLisa.ubi.x){
						case 30:
							// NDL se ubica en la posicion del vecino, entrega la flor, vuelve a su posicion anterior,
							// 	dibujo un # en el mapa de limites en la ubi del vecino y el vecino desaparece de la escena
							cuantosVecinosRecibieronFlor++;
							noviaDeLisa.ubi.y = 11;
							listaDeEscenas[6].escLimit[4][30] = '#';

							// Borro al vecino de la escena
							memcpy(&listaDeEscenas[6].escVisual[2][30], "  ", 2);
							memcpy(&listaDeEscenas[6].escVisual[3][29], "     ", 5);
							memcpy(&listaDeEscenas[6].escVisual[4][30], "  ", 2);
							break;

						case 10:
							cuantosVecinosRecibieronFlor++;
							noviaDeLisa.ubi.x = 15;
							listaDeEscenas[6].escLimit[10][10] = '#';

							memcpy(&listaDeEscenas[6].escVisual[9][7], "  ", 2);
							memcpy(&listaDeEscenas[6].escVisual[10][6], "     ", 5);
							memcpy(&listaDeEscenas[6].escVisual[11][7], "  ", 2);
							break;

						case 50:
							cuantosVecinosRecibieronFlor++;
							noviaDeLisa.ubi.x = 45;
							listaDeEscenas[6].escLimit[10][50] = '#';

							memcpy(&listaDeEscenas[6].escVisual[9][53], "  ", 2);
							memcpy(&listaDeEscenas[6].escVisual[10][51], "     ", 5);
							memcpy(&listaDeEscenas[6].escVisual[11][53], "  ", 2);
							break;
						
						default:
							break;
					}
					
					// SI ENTREGASTE LAS 3 FLORES EL MINIJUEGO TERMINA
					if (cuantosVecinosRecibieronFlor >= 3){
						impactoConEntidad = 2;
						sleep_ms(100);
						cinematica(188, 2000);
					}
				}
			}				

			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujo2, NULL);
			pthread_join(hiloEntidades2, NULL);

			noviaDeLisa.ubi = auxUbi;

			return impactoConEntidad;
			break;

		case 3:
			// TERCER VECINO - MINIJUEGO DEL LABERINTO
			noviaDeLisa.ubi = (coor){12, 8};
			actualizarHP(noviaDeLisa.hp);
			cargarEscenasConAlternativa(listaDeEscenas[7].escVisual, listaDeEscenas[7].escVisualAlt, listaDeEscenas[7].escLimit, 324);
			// char gameplayInput;

			// THREADS
			seguirDibujando = true;
			impactoConEntidad = 0;
			pthread_t hiloDibujo3;
			pthread_t hiloTimeSwitch;
			int loc5 = 5;
			pthread_create(&hiloDibujo3, NULL, threadDibujoVecino, &loc5);
			pthread_create(&hiloTimeSwitch, NULL, threadTimeSwitch, &loc5);

			while (impactoConEntidad == 0){

				noviaDeLisa.ubi = movimientoConInput(getch(), noviaDeLisa.ubi, listaDeEscenas[7].escLimit, 3);
				// sleep_ms(100); // Pequeña pausa para evitar que el thread de dibujo consuma demasiados recursos

				// CHECKEA SI LLEGASTE AL VECINO
				if (noviaDeLisa.ubi.x >= 48 && noviaDeLisa.ubi.y <= 8){
					impactoConEntidad = 2;
					sleep_ms(100);
					cinematica(273, 2000);
				}
			}

			// Detener el thread de dibujo
			seguirDibujando = false;
			pthread_join(hiloDibujo3, NULL);
			pthread_join(hiloTimeSwitch, NULL);
			timeSwitch = 0;

			noviaDeLisa.ubi = auxUbi;

			return impactoConEntidad;
			break;

		case 4:
			// CUARTO VECINO
			noviaDeLisa.ubi = (coor){12, 8};
			actualizarHP(noviaDeLisa.hp);
			cargarEscenasConAlternativa(listaDeEscenas[8].escVisual, listaDeEscenas[8].escVisualAlt, listaDeEscenas[8].escLimit, 375);
			// char gameplayInput;

			// THREADS
			// seguirDibujando = true;
			// impactoConEntidad = 0;
			// pthread_t hiloDibujo3;
			// pthread_t hiloTimeSwitch;
			// int loc5 = 5;
			// pthread_create(&hiloDibujo3, NULL, threadDibujoVecino, &loc5);
			// pthread_create(&hiloTimeSwitch, NULL, threadTimeSwitch, &loc5);

			while (impactoConEntidad == 0){

				dibujarEscena(listaDeEscenas[8].escVisual, 6);
				noviaDeLisa.ubi = movimientoConInput(getch(), noviaDeLisa.ubi, listaDeEscenas[8].escLimit, 3);
				// sleep_ms(100); // Pequeña pausa para evitar que el thread de dibujo consuma demasiados recursos

				// CHECKEA SI LLEGASTE AL VECINO
				if (noviaDeLisa.ubi.x >= 48){
					impactoConEntidad = 2;
					// sleep_ms(100);
					// cinematica(273, 2000);
				}
			}

			// Detener el thread de dibujo
			// seguirDibujando = false;
			// pthread_join(hiloDibujo3, NULL);
			// pthread_join(hiloTimeSwitch, NULL);
			// timeSwitch = 0;

			noviaDeLisa.ubi = auxUbi;

			return impactoConEntidad;
			break;

		default:
			break;
	}
}

void* threadDibujoVecino(void* arg) {
	int loc = *(int*)arg; // Convertir el argumento genérico a int

	if (loc != 5){
		while (seguirDibujando) {
			dibujarEscena(listaDeEscenas[loc+2].escVisual, loc);
									// El loc que uso para dibujarEscena es el mismo
									// 	q el index-2 de las escena de ese vecino

			// Asi entonces el thread se detiene antes de recibir el ultimo input.
			// Solo se detiene haciendo impacto con un "enemigo".
			if (impactoConEntidad != 0){
				dibujarEscena(listaDeEscenas[loc+2].escVisual, loc);
				pthread_exit(NULL);
			}

			sleep_ms(fpsDibujo);
		}
		
	// Uso el timeSwitch solo cuando estoy con el vecino 3
	} else {
		while (seguirDibujando) {
			if (timeSwitch < 2){
				dibujarEscena(listaDeEscenas[loc+2].escVisual, loc);
			} else {
				dibujarEscena(listaDeEscenas[loc+2].escVisualAlt, loc);
			}
			
			if (impactoConEntidad != 0){
				dibujarEscena(listaDeEscenas[loc+2].escVisual, loc);
				pthread_exit(NULL);
			}

			sleep_ms(fpsDibujo);
		}
	}

    return NULL;
}

void* threadActualizarUbisDeObstaculos(void* arg) {
	// depende el sistema operativo como anda frezze_ms, esta es la actual posible solucion
	int vObs = velocidadObstaculos * osMultiplicadorDeVelocidad;

    while (seguirDibujando) {
		for (int m = 0; m < 5; m++){
			// todos los obstaculos se mueven 3 a la izq
			obstaculos[m].x = obstaculos[m].x - 3;

			// el obstaculo q se pasa d largo del mapa reinicia ubi
			if(obstaculos[m].x < 10)
				obstaculos[m] = (coor){56, noviaDeLisa.ubi.y};
		}

		// noviaDeLisa.ubi.x = noviaDeLisa.ubi.x + 1;

		if (impactoConEntidad != 0)
			pthread_exit(NULL);
		
		sleep_ms(vObs);
    }
    return NULL;
}

void* threadActualizarUbisDeBalones(void* arg) {
	int vBals = velocidadBalones * osMultiplicadorDeVelocidad;

    while (seguirDibujando) {
		for (int b = 0; b < 6; b++){
			// TODOS LOS if RESTARTEAN LA UBI DE LOS BALONES SI SE PASAN DE LARGO
			// 	SINO SOLO SE MUEVEN
			switch (b){
				// ESTOS BALONES SE MUEVEN HACIA ABAJO
				case 0:
				case 5:
					if(balones[b].y < 4){
						balones[b] = (coor){noviaDeLisa.ubi.x, 15};
					} else{
						balones[b].y = balones[b].y - 1;
					}
					break;

				// ESTOS BALONES SE MUEVEN A LA DERECHA
				case 1:
				case 3:
					if(balones[b].x > 45){
						balones[b] = (coor){10, noviaDeLisa.ubi.y};
					} else{
						balones[b].x = balones[b].x + 2;
					}
					break;

				// ESTOS BALONES SE MUEVEN A LA IZQ
				case 2:
				case 4:
					if(balones[b].x < 15){
						balones[b] = (coor){50, noviaDeLisa.ubi.y};
					} else{
						balones[b].x = balones[b].x - 2;
					}
					break;
				
				default:
					break;
			}
		}

		if (impactoConEntidad != 0)
			pthread_exit(NULL);

		sleep_ms(vBals);
    }
    return NULL;
}

void* threadTimeSwitch() {
	olaUbi = -2;

    while (seguirDibujando) {

		if (timeSwitch < 2){
			timeSwitch++;
			
			olaUbi = olaUbi+3;
			for (int i = (olaUbi%2); i < maxV; i=i+2){

				memcpy(&listaDeEscenas[7].escVisualAlt[i][olaUbi-1], "   ", 3);
				memcpy(&listaDeEscenas[7].escVisualAlt[i+1][olaUbi-1], "   ", 3);

				memcpy(&listaDeEscenas[7].escVisual[i][olaUbi-1], "   ", 3);
				memcpy(&listaDeEscenas[7].escVisual[i+1][olaUbi-1], "   ", 3);

				listaDeEscenas[7].escVisual[i][olaUbi] = '~';
				listaDeEscenas[7].escVisualAlt[i][olaUbi] = '~';
			}
			
		} else{
			timeSwitch = 0;
		}

		if (impactoConEntidad != 0)
			pthread_exit(NULL);

		sleep_ms(800);
    }
    return NULL;
}















#define CABEZA "--"
#define CUERPO "./\\."
#define PIES "!!"
#define FLOR "*"

// ESTA FUNCION DIBUJA LA ESCENA SEGUN LA UBICACION DEL PERSONAJE
// 		SI NO HACES CONTACTO CON NADA DEVUELVE 0
int dibujarEscena(char d[maxV][maxH], int loc){

	// ubicarPivote();
	printf("\e[%iA", 15);

    switch(loc){
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
        case 1:											//DIBUJA LA ESCENA DE LA CASA		

			// 		ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
                for(int j = 1; j < maxH-1; j++){

					// Era una idea tipo para que solo actualize las filas de ndl pero no anda
					// 	porq primero se deberia dibujar todo una vez almenos
					// if (i > noviaDeLisa.ubi.y + 3 || i < noviaDeLisa.ubi.y - 1){
					// 	// printf("\n");
					// 	j = maxH;
					// } else 
						
						// Este if es para un mejor rendimiento del dibujo porq asi no chequea char x char
						//  y la fila que no necesita actualizacion visual la dibuja de una
					if (i > noviaDeLisa.ubi.y + 2 || i < noviaDeLisa.ubi.y){
						printf("%.60s", d[maxV-i] + 1);
						j = maxH+1;
          			
					} else if(i == noviaDeLisa.ubi.y+2 && j == noviaDeLisa.ubi.x){
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
                        printf("%c", d[maxV-i][j]);
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
            if(proximo(noviaDeLisa.ubi, curas) == 1 && noviaDeLisa.hp < 80)
				return 3;

            break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 2:											//DIBUJA LA ESCENA DE LA CALLE

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
                for(int j = 1; j < maxH-1; j++){

					// if ((i != noviaDeLisa.ubi.y) && (i != entyCoor[1].y) && (i != entyCoor[2].y) && (i != entyCoor[3].y)){
					// 	printf("%.60s", d[maxV-i] + 1);
					// 	j = maxH+1;
          			
					// } else 
					if((noviaDeLisa.calleLoc == noviaDeLisa.misionesCumplidas) && (noviaDeLisa.misionesCumplidas > 0) && ((j == entyCoor[1].x) && (i == entyCoor[1].y) || (j == entyCoor[2].x) && (i == entyCoor[2].y) || (j == entyCoor[3].x) && (i == entyCoor[3].y))){
                        printf("GB-");
						j = j + 2;

                    }else if(i == noviaDeLisa.ubi.y && j == noviaDeLisa.ubi.x){
                        // printf("L");
						printNdl(noviaDeLisa.colorL);
						if(noviaDeLisa.cantFlores > 0){
							printf("*"); 
							j++;
						}
                    } else{
                        printf("%c", d[maxV-i][j]);
                    }
                }
				// El ' ' en el printf es para borrar la flor q se buguea a la derecha de la escena de la calle.
                printf(" \n");
            }

            break;
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 3:											//DIBUJA LA ESCENA DEL VECINO 1

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
				// Segun yo este if deberia mejorar el rendimiento del dibujo
				if ((i % 2) != 0){
					// Printea un String de 60 caracteres sin contar el primero (que es un #)
					printf("%.60s", d[maxV-i] + 1);
					
				} else {
					for(int j = 1; j < maxH-1; j++){
	
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
							printf("_|C");
							// printf("@@@");
							
							// if((j == noviaDeLisa.ubi.x-1) && (i == noviaDeLisa.ubi.y))
							if(proximo(noviaDeLisa.ubi, (coor){j+1, i}) > 0){
								// noviaDeLisa.hp = noviaDeLisa.hp - 25;
								impactoConEntidad = 1;
							}
	
							j = j+2;
	
						}else if((j == noviaDeLisa.ubi.x) && (i == noviaDeLisa.ubi.y) && impactoConEntidad == 0){
							printNdl(noviaDeLisa.colorL);
							printf("*");
							j++;
	
						} else{
							printf("%c", d[maxV-i][j]);
						}
					}
				}
				// este prntf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf("               ");
                printf("\n");
            }

			break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 4:											//DIBUJA LA ESCENA DEL VECINO 2

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
				
                for(int j = 1; j < maxH-1; j++){
					if (((j == balones[0].x+1) && (i == balones[0].y+1)) || ((j == balones[5].x) && (i == balones[5].y+1))){
						printf("s");

					}else if(((j == balones[0].x) && (i == balones[0].y)) || ((j == balones[5].x) && (i == balones[5].y))){
                        printf("()");
						
						if (proximo(noviaDeLisa.ubi, (coor){j, i}) >= 2){
							// noviaDeLisa.hp = noviaDeLisa.hp - 25;
							impactoConEntidad = 1;
						}
						j = j+1;
						
                    }else if (((j == balones[1].x) && (i == balones[1].y)) || ((j == balones[3].x) && (i == balones[3].y))){
						printf("~O)");
						
						if (proximo(noviaDeLisa.ubi, (coor){j+1, i}) >= 1){
							// noviaDeLisa.hp = noviaDeLisa.hp - 25;
							impactoConEntidad = 1;
						}
						j = j+2;
						
					}else if(((j == balones[2].x-1) && (i == balones[2].y)) || ((j == balones[4].x-1) && (i == balones[4].y))){
						printf("(O~");
						
						if (proximo(noviaDeLisa.ubi, (coor){j, i}) >= 1){
							// noviaDeLisa.hp = noviaDeLisa.hp - 25;
							impactoConEntidad = 1;
						}
						j = j+2;
						
					}else if((j == noviaDeLisa.ubi.x) && (i == noviaDeLisa.ubi.y)){
						printNdl(noviaDeLisa.colorL);
                        printf("*");
						j++;
						
                    } else{
                        printf("%c", d[maxV-i][j]);
                    }
                }
				// el espacio en este printf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf(" \n");
            }

			break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 5:											//DIBUJA LA ESCENA DEL VECINO 3

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
				
                for(int j = 1; j < maxH-1; j++){
					if((j == noviaDeLisa.ubi.x) && (i == noviaDeLisa.ubi.y)){
						printNdl(noviaDeLisa.colorL);
						printf("*");
						j++;

                    } else{
                        printf("%c", d[maxV-i][j]);
                    }
                }
				// este prntf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf("\n");
            }

			if (olaUbi >= noviaDeLisa.ubi.x){
				// Le meto freeze para q se alcanze a dibujar una ola sobre L antes de frenar los threads
				sleep_ms(200);
				impactoConEntidad = 1;
			}

			break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		case 6:											//DIBUJA LA ESCENA DEL VECINO 4

			// ESTE FOR SOLO DIBUJA LA ESCENA
            for(int i = maxV-1; i > 1; i--){
                printf("                              ");
				
                for(int j = 1; j < maxH-1; j++){
					if((j == (noviaDeLisa.ubi.x))){
						if (i == noviaDeLisa.ubi.y+2 || i == noviaDeLisa.ubi.y-2){
							printf("\033[2D");
							// printf("      ");
							printf("%.6s", listaDeEscenas[8].escVisualAlt[maxV-i]+(j-2));
							j = j+3;
						} else if (i == noviaDeLisa.ubi.y+1 || i == noviaDeLisa.ubi.y-1){
							printf("\033[3D");
							// printf("        ");
							printf("%.8s", listaDeEscenas[8].escVisualAlt[maxV-i]+(j-3));
							j = j+4;
						} else if (i == noviaDeLisa.ubi.y){
							printf("\033[3D");
							// printf("   ");
							printf("%.3s", listaDeEscenas[8].escVisualAlt[maxV-i]+(j-3));
							printNdl(noviaDeLisa.colorL);
							printf("*");
							// printf("   ");
							printf("%.3s", listaDeEscenas[8].escVisualAlt[maxV-i]+(j+2));
							j = j+4;
						} else{
                        	printf("%c", d[maxV-i][j]);
						}

                    } else{
                        printf("%c", d[maxV-i][j]);
                    }
                }
				// este prntf es para borrar el obstaculo q se buguea a la derecha del vecino
                printf("\n");
            }

			break;

        default:
            break;
    }
    return 0;
}