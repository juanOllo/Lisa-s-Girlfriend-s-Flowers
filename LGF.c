#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <pthread.h>

#include <assert.h>
#include <unistd.h>

//#include <windows.h>

#define SIZE 200

int cond = 0;

/////////////////////////NUEVO GAMEPLAY LEXICO ////////////////////

int actualLocation;

#define max 16
#define max2 60

typedef struct{
    int x;
    int y;
} coor;

coor ubi;					//ubi de ndl
coor eubi, eubi2, eubi3;	//enemigos/abejas
coor vubi1, vubi2; 			//entradas de vecinos
coor cubi; 					//ubicasion de puerta de casa/hogar

coor fubi, pubi, hubi;		//ubis dentro de la casa

typedef struct{
	char data[500];
	int segsEnPantalla;
} mensajesEnPantallas;

mensajesEnPantallas mensajeInformativo;

char escCalle1[max][max2];
char escCalle2[max][max2];
char escCalle3[max][max2];
char escCasa[max][max2];
char casaDeVecino[max][max2];

char escCasaLimit[max][max2];
char escCalleLimit[max][max2];

char llamada[max][max2];	//NO SIRVE PERO SI SE BORRA SE ROMPE EL PROGRAMA :D

int auxCont;

void freeze(int s);			//pausa la pantalla

///////////////////////////////////////////////////////////////////

struct NoviaDeLisa {
	int HP; // NDL.HP
	int cantFlores;
	int misionesCumplidas;
//	int TEP;
	int primeraVez;
	int lucides[3];
};

struct NoviaDeLisa NDL;

FILE* archivo;

/*	POR BORRAR	*/void MenuCombate(struct NoviaDeLisa *ndl);

		///		ACCIONES Y FUNCIONES		///
int MenuDeCasaNuevo(struct NoviaDeLisa *ndl);///////////NUEVO////////

void MenuDeCalle(struct NoviaDeLisa *ndl, int *o);//NUEVO
int MenuCasasNuevo();//NUEVO
void menuDeCombateNuevo();//NUEVO

void CartelSalir();//VIEJO
void endGame(struct NoviaDeLisa *ndl);//VIEJO
int gameOver();//VIEJO

		///		ACCIONES Y FUNCIONES AUXILIARES		///
void cargaDeEscena(char d[max][max2], int l/*line*/);
void cinematica(int l/*line*/);
int dibujo(char d[max][max2], int loc);
void randomUbi(coor *e);
int proximo(coor u, coor e);
void leerEscuchar(int cont);

		///		ACCIONES GAMEPLAY NO STOP		///
void *procesoSeparado(void *data);

void *thread_test_run (void *v);

void main(){

	strcpy (mensajeInformativo.data, "");
	mensajeInformativo.segsEnPantalla = 0; 

    auxCont = 0;
	/////////////////////////CARGA DE ESCENAS/////////////////
    /**/ cargaDeEscena(escCalle1, 1);
	/**/ cargaDeEscena(escCalle2, 69);
	/**/ cargaDeEscena(escCalle3, 86);
    /**/ cargaDeEscena(escCasa, 35);
	/**/ cargaDeEscena(casaDeVecino, 120);
	/**/ 
	/**/ cargaDeEscena(escCalleLimit, 18);
	/**/ cargaDeEscena(escCasaLimit, 52);
	/////////////////////////////////////////////////////////

	//////////////////INICIALISACION DE UBICACIONES////////////////// 
	/*		LOCS DENTRO DE LA CASA		*/
	/**/ ubi.x = 31;		//NDL por primera vez en el juego
    /**/ ubi.y = 6;
	/**/
    /**/ fubi.x = 46;	//FLOR
    /**/ fubi.y = 10;
    /**/
    /**/ pubi.x = 56;	//PUERTA A LA CALLE
    /**/ pubi.y = 6;
    /**/
    /**/ hubi.x = 23;	//CURAS (HEALT)
    /**/ hubi.y = 4;
	////////////////////////////////////////////////////////////////

	/////////////////INICIALISACION DE PESONAJE/////////////////////
	/**/ NDL.HP = 100;
	/**/ NDL.cantFlores = 0; //0
	/**/ NDL.misionesCumplidas = 0;
	/**/ //NDL.TEP = 0;
	/**/ NDL.primeraVez = 0;
	/**/ for(int ini = 0; ini<=3; ini++){
	/**/ 	NDL.lucides[ini] = 0;
	/**/ }
	////////////////////////////////////////////////////////////////
		/*		ESTO ES DEL TEMA DE LOS HILOS
		pthread_t proceso1;
			pthread_create (&proceso1, NULL, &procesoSeparado, NULL);
			pthread_kill(proceso1, NULL);
			freeze(5);
    		pthread_cancel (proceso1);
		*/

	/*for (int i=0; i<=max; i++){					//LO USO PARA CHEKEAR LA CARGA DE LAS ESCENAS
        for(int j=0; j<=max2; j++){
            printf("%c", escCalle2[i][j]);
        }
        printf("\n");
    }*/
    
	/*
	freeze(1);
	printf("\n\n\n\n\n\n\n\n\n");
	printf("                              ");
	printf("               ");
    printf("LISA'S ");
	freeze(2);
	printf("GIRLFRIEND'S ");
	freeze(2);
	printf("FLOWERS ");
	freeze(3);
	*/
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n Presiona una tecla para jugar..\n");		
	
	printf("\n 27 \n");
	char auxSkip/*auxiliar para saltar cinematicas, no sirve para nada*/ = getch();
	
	/*
	cinematica(1);
	freeze(5);
	printf("\n\n\n\n\n\n Presiona una tecla para continuar\n");
	auxSkip = getch();
	

	system("cls");
	//freeze(1);
	cinematica(18);
	freeze(2);
	*/		
    

	srand(time(NULL));//Sirve para q rand() no de siempre los mismos numeros randoms

	//
	int op = 1;
	char opp;
	do{
		switch(op){
			case 1: {
			    op = MenuDeCasaNuevo(&NDL);
			    printf("\nop: %d\n", op);
				//MenuDeCasa(&NDL, &op);
				break;
			};
			case 2: {
				cargaDeEscena(escCalleLimit, 18);
				MenuDeCalle (&NDL, &op);
				break;
			}
			case 3: {
				op = gameOver();
				break;
			}
			case 4: {
				break;
			}
			case 5: {
				break;
			}
			case 6: {
				op = 1;
				break;
			}
			case 7: {
				CartelSalir();
				break;
			}
			default: {
				printf("Ingrese un valor entre 1 y 7.\n");
			}	

		}
	}while(op != 7);

};

///////////////////////////ACCIONES QUE SERAN ELIMINADASSS///////////////////////////////////////////////////////////////////////////

void MenuCombate(struct NoviaDeLisa *ndl){
	printf("\t CASA de ");
	leerEscuchar(ndl->misionesCumplidas+1);
	printf("HP: %d \n\n", ndl->HP);
	printf("1. DAR FLOR\n");
	printf("2. ESCUCHAR (-25HP)\n");
	printf("3. Salir a la calle\n");
}

int gameOver(){
	int r;
	printf("\tGAME OVER\n");
	printf("1. Continuar\n7. Salir\n");
	scanf("%d", &r);
	return r;
}


//////////////////////////////////////////////////////////////

/////////////////////////////////ACCIONES///////////////////////////////////////////////////////////////////////////////

int MenuDeCasaNuevo(struct NoviaDeLisa *ndl){
    
    /*if(ndl->primeraVez == 0){
		printf("[LISA al telefono]: . . .");
		ndl->primeraVez = 1;
	}*/
    
    //cargaDeEscena(esc);
    
    //srand(time(NULL));
    
    int cond = 0;
    int caux = 0;

	//ubi.x = 52;
	//ubi.y = 6;
    
    char opc;
    
    //system ("/bin/stty raw"); //FUNCIONA PARA Q GETCHAR() FUNCIONE EN UBUNTU
    
    while(opc != 'p'){
        system("cls");
        //printf("Ubicacion: %d %d \n", ubi.x, ubi.y); //Ubicasion de la novia d lisa
        //printf("Ubicacion enemi: %d %d \n", eubi.x, eubi.y); //ubicasion de las abejas
        
        cond = dibujo(escCasa, 1);
        
        
        //printf("\n/\tESTAS EN CASA:\t     \\");
        printf("\n\n\n\t\t\t\t\t   <3 HP: %d <3", ndl->HP);
        printf("\tFLOR EN MANO: %d \n\n", ndl->cantFlores); 
        
        //Unica utilizacion de int primeraVez
		if((ndl->HP < 100) && (ndl->primeraVez == 0)){
			//printf("\n[Mensaje de Lisa]:Dejé mas caramelos en tu habitacion para recuperar energia, \n\trecuerda que solo debes entregar las flores.\n\tPd.: Gracis por ayudarme tq :3\n");
			strcpy(mensajeInformativo.data, "[Mensaje de Lisa]:Dejé mas caramelos en la mesa para recuperar energia, \n\trecuerda que solo debes entregar las flores.\n\tPd.: Gracis por ayudarme tq :3");
			mensajeInformativo.segsEnPantalla = 3;
			ndl->primeraVez = 1;
		}
        
        switch(cond){
            case 0:
                opc = getch();
                break;
            case 1:
				ubi.x = 11;
    			ubi.y = 6;
                return 2;
                break;
            case 2:
                //printf("\nTomaste una flor.\n");
				strcpy(mensajeInformativo.data, "\nTomaste una flor.\n");
				mensajeInformativo.segsEnPantalla = 1;
                ndl->cantFlores = 1;
                opc = getch();
                break;
            case 3:
                //printf("\nTomaste las pastillas\n");
				strcpy(mensajeInformativo.data, "\nTomaste una de las pastillas.\n");
				mensajeInformativo.segsEnPantalla = 2;
                ndl->HP = 100;
                opc = getch();
                break;
            default:
                opc = getch();
                break;
        }
        
        
	    switch(opc){
	        case 'd':
				if((escCasaLimit[max - ubi.y][ubi.x+3] != '#')){//&&(escCasaLimit[max - ubi.y][ubi.x+2] != '#')){
	            	ubi.x = ubi.x + 3;
				}
	            break;
            case 'a':
				if((escCasaLimit[max - ubi.y][ubi.x-3] != '#')){//&&(escCasaLimit[max - ubi.y][ubi.x-2] != '#')){	
	            	ubi.x = ubi.x - 3;
				}
	            break;
            case 'w':
				if(escCasaLimit[max - ubi.y - 1][ubi.x] != '#'){
	            	ubi.y = ubi.y + 1;
				}
	            break;
            case 's':
				if(escCasaLimit[max - ubi.y + 1][ubi.x] != '#'){
	            	ubi.y = ubi.y - 1;
				}
	            break;
			case 'm':
				if(NDL.misionesCumplidas < 3){
						if(NDL.cantFlores == 0){
							cinematica(205);
							freeze(2);
						}
						else{
							cinematica(222);
							freeze(2);
						}
				}	
				else{				////CUANDO TENES MAS DE x MISIONES CUMPLIDAS MUESTRA MANO LASTIMADA
					if(NDL.cantFlores == 0){
						cinematica(239);
						freeze(2);
					}
					else{
						cinematica(256);
						freeze(2);
					}
				}
				break;
            default:
                break;
	    }
    }
    
}

void MenuDeCalle (struct NoviaDeLisa *ndl, int *o){

	//system ("cls");
	int nroCasa = 1;
	

	do{
	    
	    if(ndl->HP <= 0){
			nroCasa = 7; 
		}
        else{ 
	        printf("\n\tESTAS EN LA CALLE:\n\n");
			printf("misionescumplidas: %d", ndl->misionesCumplidas);
			    system("cls");
				actualLocation = nroCasa;
	            nroCasa = MenuCasasNuevo();
    	        //MenuCasas();
				//scanf("%d",&nroCasa);
				printf("\nnroCasa: %d\n", nroCasa);
        }
        
        int contador001;
		
		if ((0 < nroCasa) && (nroCasa <=4) ){
			contador001 = 1;
		}
		else {
		    contador001 = nroCasa;
		}
		
		printf("misionescumplidas: %d", ndl->misionesCumplidas);
		printf("misionescumplidas: %d", ndl->HP);
		printf("misionescumplidas: %d", ndl->cantFlores);

		switch(contador001){
		    case 1:{
		        if(nroCasa-1 > ndl->misionesCumplidas){
		            //system("cls");
		            printf("XXXX Debes entregar la flor anterior XXXX\n");
		        }
		        else{
    		        if(nroCasa <= ndl->misionesCumplidas){
                        ubi.y = ubi.y - 1; 
						//system ("cls");
    	        	    strcpy (mensajeInformativo.data, "\nEsta flor ya fue entregada\n");
                		if (ndl->lucides[nroCasa-1] == 1){
							strcpy (mensajeInformativo.data, "\nEsta flor ya fue entregada\n(alguien entrego la flor por ti)\n");
                		}
						mensajeInformativo.segsEnPantalla = 2;
    	        	}
    		        else{
    	        	    //system ("cls");
    	        	    int AuxMisionesCumplidas = ndl->misionesCumplidas;
    	        		menuDeCombateNuevo();
    	        		if(ndl->misionesCumplidas > AuxMisionesCumplidas){
    	        		    *o = 1;
    	        		    nroCasa = 6;
    	        		}
    	        		    
    	        	}
		        }
		        break;
		    }
		    case 5: {
				system ("cls");
				if (ndl->misionesCumplidas < 4){printf("X Debes entregar todas las flores X\n");}
				else {
					endGame(ndl);
					*o = 7;
					nroCasa = 6;
				}
				break;
			}
		    case 6: {
				*o = 1;
				break;
			}
			case 7: {
				//ndl->HP = 1;
				//ndl->misionesCumplidas = 0;
				ndl->cantFlores = 0;
				*o = 3;
				nroCasa = 6;
			}
		    
		}
		
	}while(nroCasa != 6);
}

int MenuCasasNuevo(){
	    
    //cargaDeEscena(escCalleLimit, 18);	//comentado para solucionar error de escCalleLimit siempre de calle 1
    
    //srand(time(NULL));
    
    int cond = 0;
    int caux = 0;
	//actualLocation = 1;
    
    char opc;
    
    eubi.x = 30;
    eubi.y = 7;
    
    eubi2.x = 18;
    eubi2.y = 10;
    
    eubi3.x = 45;
    eubi3.y = 4;
    
    //ubi.x = 11;
    //ubi.y = 6;
    
    vubi1.x = 35;
    vubi1.y = 13;
    
    cubi.x = 9;
    cubi.y = 6;

    vubi2.x = 30;
    vubi2.y = 12;

	pthread_t proceso1;

    //system ("/bin/stty raw"); //para que getchar funcione en ubuntu
    
    while(opc != 'p'){
        system("cls"); //descomentar
        //printf("Ubicacion: %d %d \n", ubi.x, ubi.y); //Ubicasion de la novia d lisa
        //printf("Ubicacion enemi: %d %d \n", eubi.x, eubi.y); //ubicasion de las abejas
        
		switch(actualLocation){
			case 0:
				cond = 6;
			case 1:
				/*
    			pthread_create (&proceso1, NULL, &procesoSeparado, "h");
				printf(" se vino ");
    			pthread_cancel (proceso1);
				printf(" se vino ");*/
				cond = dibujo(escCalle1, 2);
				break;
			case 2:
				cond = dibujo(escCalle2, 2);
				break;	
		}
		printf("Ubicacion: %d %d \n", ubi.x, ubi.y); //Ubicasion de la novia d lisa
		//printf("\nescCalleLimit: %c\n", escCalleLimit[max - ubi.y+1][ubi.x]);
		//printf("Ubicacion: %d %d \n", ubi.x, ubi.y);
        
		//printf(" se fue ");

        switch(cond){
            case 0:
                randomUbi(&eubi);
                randomUbi(&eubi2);
                randomUbi(&eubi3);
                opc = getch();
				//pthread_cancel (proceso1);
				//freeze(1); //Idea de juego reproduciendose sin input 
                break;
            case -1:
                printf("\nTE ATRAPARON\n");
				cinematica(18);
				freeze(2);
                //return 6; //Vuelve a casa desp que lo atrapan
                //opc = 'p'; //Vuelve a la puerta de casa desp que lo atrapan
				//actualLocation = 0; //Para volver a casa luego de ser atrapado por una abeja
				NDL.cantFlores = 0;
				ubi.x = 52;
    			ubi.y = 6;
                return 6;
                break;
            case 1:
                ubi.x = 59;
				actualLocation = 1;
				cargaDeEscena(escCalleLimit, 18);
                break;
			case 2:
				ubi.x = 1;
				actualLocation = 2;
				cargaDeEscena(escCalleLimit, 86);
				break;
			case 11:
				return 1;
				break;
			case 12:
				return 2; //Numero de la casa a la q entro
				break;
            case 6:
				ubi.x = 52;
    			ubi.y = 6;
                return cond;
				break;
        }
        
	    switch(opc){
	        case 'd':
				if((escCalleLimit[max - ubi.y][ubi.x+1] != '#')&&(escCalleLimit[max - ubi.y][ubi.x+2] != '#') || (ubi.x == 59 && NDL.misionesCumplidas > 0)){
	            	ubi.x = ubi.x + 2;
				}
	            break;
            case 'a':
				if((escCalleLimit[max - ubi.y][ubi.x-1] != '#')&&(escCalleLimit[max - ubi.y][ubi.x-2] != '#') || (ubi.x == 1 && actualLocation > 1)){	
	            	ubi.x = ubi.x - 2;
				}
	            break;
            case 'w':
				if(escCalleLimit[max - ubi.y - 1][ubi.x] != '#'){
	            	ubi.y = ubi.y + 1;
				}
	            break;
            case 's':
				if(escCalleLimit[max - ubi.y + 1][ubi.x] != '#'){
	            	ubi.y = ubi.y - 1;
				}
	            break;
			case 'm':
				if(NDL.misionesCumplidas < 3){
						if(NDL.cantFlores == 0){
							cinematica(205);
							freeze(2);
						}
						else{
							cinematica(222);
							freeze(2);
						}
				}	
				else{				////CUANDO TENES MAS DE x MISIONES CUMPLIDAS MUESTRA MANO LASTIMADA
					if(NDL.cantFlores == 0){
						cinematica(239);
						freeze(2);
					}
					else{
						cinematica(256);
						freeze(2);
					}
				}
				break;
            default:
                break;
	    }
    }
    //char opc = getchar();
	//int op = atoi(&opc);
}

void menuDeCombateNuevo(){

	int opcionDeCombate;
	int contador = (6 * NDL.misionesCumplidas)+1;
	char msj[50];
	char auxOpcionDeCombate;

	strcpy(msj, " ");

	do{
		
		if(NDL.HP <= 0/*ndl->TEP > ndl->misionesCumplidas*/){ 
			opcionDeCombate = 5;
			cinematica(103 + ((17*5) * (NDL.misionesCumplidas-1)));
			//printf("\n");
			leerEscuchar(contador);
			freeze(3);
		}else{

			if(NDL.cantFlores < 1){
				cinematica(35 + ((17*5) * NDL.misionesCumplidas));	//dibujo(casaDeVecino, 3);	
			}else{
				cinematica(103 + ((17*5) * NDL.misionesCumplidas));
			}
			
			//printf("\n");
			leerEscuchar(contador);
			//printf("\t\t\t\t1.Dar Flor\t2.Escuchar\t3.Salir a la calle\n");
			printf("\t\t\t                 .n.                    /.  \\               |     |\n");
			printf("\t\t\t                C O D                  / )  /               |  |  |\n");
			printf("\t\t\t    1 .Dar Flor  *Y*      2 .Escuchar .)(-{      3 .Volver  |     |\n");
			printf("\t\t\t                  |                    \\_b                  |  |  |\n");
			printf("%s", msj);

			auxOpcionDeCombate = getch();
			opcionDeCombate = atoi(&auxOpcionDeCombate);
			//scanf("%d", &opcionDeCombate);
		}
		//if(opcionDeCombate == 6){ opcionDeCombate = 6;};
		if((NDL.cantFlores == 0) && (opcionDeCombate != 3) && (NDL.HP > 0)){ opcionDeCombate = 4;};
		switch(opcionDeCombate){
			case 1: {
				strcpy(msj, "*Entregaste la flor*\n");
				cinematica(103 + ((17*5) * NDL.misionesCumplidas));	//dibujo(casaDeVecino, 3);
				freeze(1);
				cinematica(52 + ((17*5) * NDL.misionesCumplidas));
				freeze(1);
				cinematica(69 + ((17*5) * NDL.misionesCumplidas));
				freeze(1);
				cinematica(86 + ((17*5) * NDL.misionesCumplidas));
				freeze(2);
				NDL.misionesCumplidas = NDL.misionesCumplidas + 1;
				NDL.cantFlores = NDL.cantFlores - 1;
				ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia cinesion en la casa
				ubi.y = 6;
				break;
			}
			case 2: {
				system("cls");
				//leerEscuchar(contador);
				NDL.HP = NDL.HP - 25;
				contador = contador + 1;
				if ( NDL.HP <= 0/*contador > ((ndl->misionesCumplidas + 5) +12)*/){
					//printf("\ncontador: %d\n", contador);
					NDL.lucides[NDL.misionesCumplidas] = 1;
					//ndl->TEP = ndl->TEP + 1;
					//printf("\nfin\n");
					//opcionDeCombate = 2;
					//break;
					NDL.misionesCumplidas = NDL.misionesCumplidas + 1;
					NDL.cantFlores = 0;
					ubi.x = 31;		//Cuando vuelve a casa desp de entregar una flor (o q la entreguen) reinicia ubicasion en la casa
					ubi.y = 6;
				}
				else {
					//MenuCombate(ndl);
				}
				break;
			}
			case 3: {
				//system("cls");
				//printf("Saliste a la calle . .\n");
				ubi.x = 35;
				ubi.y = 12;
				break;
			}
			case 4: {
				system("cls");
				printf("No llevas flores contigo ahora!!!\n\n");
				strcpy(msj, "No llevas flores contigo ahora!!!\n");
				//MenuCombate(ndl);
				break;
			}
			case 5: {
				//printf("\nFIN DEL JUEGO\n");
				opcionDeCombate = 7;
				break;
			}
		}
	}while( opcionDeCombate % 2 == 0/*(opcionDeCombate == 2) || (opcionDeCombate == 4)*/);
}

void endGame(struct NoviaDeLisa *ndl){

    int cantEscuchar = 0;
	int egpCont = 26;
	int TEP = 0;
	for(int ini3 = 0; ini3<=3; ini3++){
		TEP = TEP + NDL.lucides[ini3];
	}
	//printf("\nTEP: %d \n", TEP);
	printf("\tCASA DE LISA\n\n");
	leerEscuchar(egpCont);

	TEP = 40;
	
	int egp;
	do{
		
		printf("1.Aceptar la flor de Lisa    ");
		if((TEP > 1) || (cantEscuchar ==3)){
			printf("2.No aceptar la flor    ");
		}
		if(egpCont >=29){
			printf("3.Matar a Lisa");
		}
		printf("\n\n");
		scanf("%d", &egp);

		if(TEP >= 0){
			switch(egp){
				case 1:{
				    system("cls");
                	printf("*Recibiste la flor de Lisa*\n\n");
					break;
				}
				case 2:{
				    system("cls");
					printf("\tCASA DE LISA\n\n");
				    if(TEP > 1){
				        egpCont = egpCont + 1;
				        cantEscuchar = cantEscuchar + 1;
				        TEP = TEP - 1;
				    }
					leerEscuchar(egpCont);
					break;
				}
				case 3:{
				    if(cantEscuchar == 3){
						system("cls");
						printf("la mataste weon\n\n");
						egp = 5;
				    }
				    else{
				        system("cls");
					    printf("\tCASA DE LISA\n\n");
					    leerEscuchar(egpCont);
				    }
				    break;
				}
			}
		}
	}while((egp != 1) && egp != 5);

	CartelSalir();
	printf("\n");
}

//Muestra un mensaje al salir del programa
void CartelSalir(){
	printf("       |------------------------|\n");
	printf("       |  ¡Gracias por jugar!   |\n");
	printf("       |------------------------|\n");
}

//////////////////ACCIONES NO GAMEPLAY//////////////////////////////////////////////////////////////////////

void leerEscuchar(int cont){

	char frase[SIZE];
	char dialogo[SIZE];
	int i = 1;
	archivo= fopen("escuchar.txt","r");
	//printf("\nEntro a escuchar.\n");
	
	if (archivo!=NULL){
		while(!feof(archivo)){
			fgets(frase, sizeof(frase), archivo);	
			while (i <= cont){
			//switch(i){
			//	case 1 :{
					//printf("\nFrase01: %s\n", frase);
					strcpy( dialogo, frase);
					i++;
					break;
			//	}
			//}
			}
		}
	}//while(i != 0);
	printf("%s\n", dialogo);
	//printf("\nTermina escuchar.\n");

}

int dibujo(char d[max][max2], int loc/*Ubicacion de ndl*/){
    
    /*
    for (int i=1; i<=max; i++){
        for(int j=1; j<=max*2; j++){
            printf("%c", d[i][j]);
        }
        printf("\n");
    }*/
	
    int j, i;
    
	int rAux;

    system("cls");
    printf("\n\n\n\n\n\n\n");
    switch(loc){
        case 1:											//DIBUJA LA ESCENA DE LA CASA
            for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
					if(j == ubi.x && i == ubi.y+2){
						printf("--");
						j++;
					}else if(j == ubi.x-1 && i == ubi.y+1){
                        printf("./\\.");
						if(NDL.cantFlores > 0){
							printf("*");
							j++;
						}
                        j = j + 3;
                    }else if(j == ubi.x && i == ubi.y){
                        printf("!!");
						j++;
                    }else if((j == fubi.x) && (i == fubi.y)){
                        printf("\\");
                    }else if((j == hubi.x) && (i == hubi.y)){
						printf(".");
					}
					
					/**/
					else if((j == 44) && (i == 11) && (NDL.misionesCumplidas + NDL.cantFlores < 1)){
						printf("*");
					}else if((j == 50) && (i == 12) && (NDL.misionesCumplidas + NDL.cantFlores < 2)){
						printf("*");
					}
					/**/
					
					else{
                        printf("%c", d[max-i][j]);
                    }

					/**/
					if (7 == ubi.x && 1 == ubi.y){
						endGame(&NDL);
					}
					/**/
                }
                
                printf("\n");
            }
            
			if(strcmp(mensajeInformativo.data, "") != 0){
				printf("%s", mensajeInformativo.data);
				strcpy(mensajeInformativo.data, "");
				freeze(mensajeInformativo.segsEnPantalla);	
			}
            
            if(proximo(ubi, pubi) == 1){
                return 1;	//Salir por la puerta
            }
            if(proximo(ubi, fubi) == 1 && NDL.cantFlores == 0){
                return 2;	//Agarrar una flor 
            }
            if(proximo(ubi, hubi) == 1 && NDL.HP < 100){
                return 3;	//Tomar un pastilla
            }

			//printf("%s", mensajeInformativo);
            
            break;
        case 2:											//DIBUJA LA ESCENA DE LA CALLE
            for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
                    if((actualLocation == NDL.misionesCumplidas) && (NDL.misionesCumplidas > 0) && ((j == eubi.x) && (i == eubi.y) || (j == eubi2.x) && (i == eubi2.y) || (j == eubi3.x) && (i == eubi3.y))){
                        printf("GB-");
                        j = j + 2;
                    }else if((j == ubi.x) && (i == ubi.y)){
                        printf("L");
						if(NDL.cantFlores > 0){
							printf("*"); 
							j++;
						}
                    }
                    /*else if((j == tubi.x) && (i == tubi.y)){
                        printf("x");
                    }*/else{
                        printf("%c", d[max-i][j]);
                    }
                }
                
                printf("\n");
            }

			if(strcmp(mensajeInformativo.data, "") != 0){
				printf("%s", mensajeInformativo.data);
				strcpy(mensajeInformativo.data, "");
				freeze(mensajeInformativo.segsEnPantalla);	
			}
            
			if(actualLocation == 1 && ubi.x > 59){
				return 2;
			}
			if(actualLocation == 2 && ubi.x < 1){
				return 1;
			}
            if(proximo(ubi, cubi)==2 && actualLocation==1){
                return 6;
            }

			//
			if(proximo(ubi, vubi2)>0 && actualLocation==2){
				return 12; //entra al vecino 2
			}
			//

            if(((actualLocation == NDL.misionesCumplidas)) && (NDL.cantFlores > 0 && NDL.misionesCumplidas > 0) && ((proximo(ubi, eubi) > 0) || (proximo(ubi, eubi2) > 0) || (proximo(ubi, eubi3) > 0))){
                return -1;
            }

            if(proximo(ubi, vubi1) == 2 && actualLocation==1){
                return 11;	//el vecino d la calle 1
            }
            break;
		case 3:										////DIBUJA LA ESCENA DE LOS VECINOS
			for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
					if((NDL.cantFlores > 0) && (j==10) && (i==7)){
						printf("*");
						j++;
					}
                    printf("%c", d[max-i][j]);
                }
                printf("\n");
            }
			break;
        case 9:												//DIBUJA LAS CINEMATICAS
            for(i = max-1; i > 0; i--){
                printf("                                   ");
                for(j = 1; j < max2; j++){
                    
                        printf("%c", d[max-i][j]);

                }
                printf("\n");
            }
            
            //freeze( ); //por si pinta q todas las cinematicas duren lo mismo
    }
    return 0;
}

void randomUbi(coor *e){ //Le da una ubicasion random a e dentro del area permitida
    int r;
    
    r = rand() % 9;
    //printf("\n random: %d", r); //Proximo movimiento de la abeja
    switch(r){
        case 0:
            if(escCalleLimit[max - e->y - 1][e->x] != '#'){//e->y < max-3){
                e->y = e->y + 1;
            }
            break;
        case 1:
            if(escCalleLimit[max - e->y][e->x-1] != '#'){//e->x > 1){
                e->x = e->x - 1;
            }
            break;
        case 2:
            if(escCalleLimit[max - e->y + 1][e->x] != '#'){//e->y > 1){
                e->y = e->y - 1;
            }
            break;
        case 3:
            if(escCalleLimit[max - e->y][e->x+1] != '#'){//e->x < max2){
                e->x = e->x + 1;
            }
            break;
        case 4:
            if((escCalleLimit[max - e->y - 1][e->x] != '#') && (escCalleLimit[max - e->y][e->x-1] != '#')){//(e->y < max-3)&&(e->x > 1)){
                e->y = e->y + 1;
                e->x = e->x - 1;
            }
            break;
        case 5:
            if((escCalleLimit[max - e->y - 1][e->x] != '#') && (escCalleLimit[max - e->y][e->x+1] != '#')){//(e->y < max-3)&&(e->x < max2)){
                e->y = e->y + 1;
                e->x = e->x + 1;
            }
            break;
        case 6:
            if((escCalleLimit[max - e->y + 1][e->x] != '#') && (escCalleLimit[max - e->y][e->x-1] != '#')){//(e->y > 1)&&(e->x > 1)){
                e->y = e->y - 1;
                e->x = e->x - 1;
            }
            break;
        case 7:
            if((escCalleLimit[max - e->y + 1][e->x] != '#') && (escCalleLimit[max - e->y][e->x+1] != '#')){//(e->y > 1)&&(e->x < max2)){
                e->y = e->y - 1;
                e->x = e->x + 1;
            }
            break;
        default:
            break;
    }
}

int proximo(coor u, coor e){
    
    if((e.x+4>u.x && u.x>e.x-2) && (e.y+2>u.y && u.y>e.y-2)){
        if(e.x==u.x && e.y==u.y){
            return 2; //Están en el mismo lugar 
        }
        return 1; //U está en el area de e pero no en el mismo lugar
    }
    return 0; //No están ni cerca
}

void cargaDeEscena(char d[max][max2], int l/*line*/){
    FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    int inicio = 0;
    char linea[max2+2];
    
    f = fopen("escenas.txt", "r");
    
    while(inicio < l){
        fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        fscanf(f, "%c", &aux);
        inicio++;
    }
    
    if(f != NULL){
		
        for (i=1; i<=max; i++){
			d[i][0] = '#';
			//d[i][max] = 'j';
            fscanf(f, "%c", &aux);
            for(j=1; j<=max2; j++){
                fscanf(f, "%c", &d[i][j]);
            }
            fscanf(f, "%c", &aux);
            fscanf(f, "%c", &aux);
        }
		j=0;
		while(j < max2){
		d[max][j] = '#';
		d[0][j] = '#';
		j++;
		}
		
    }
    else{
        printf("error");
    }

	/*
	for(i=0; i <= max+1; i++){
		for(j=0)
	}*/

	fclose(f);
}

void freeze(int s){//Pausa la pantalla s segundos
	while(s>0){
        
        	clock_t stop = clock() + CLOCKS_PER_SEC;
        	while(clock() < stop){}
        
        	s --;
    		}
}

void cinematica(int l/*line*/){
	FILE *f;
    int i = 0;
    int j = 0;
    char aux;
    int inicio = 0;
    char linea[max2+1];
    
    f = fopen("cinematicas.txt", "r");
    
    while(inicio < l){
        fscanf(f, "%c", &aux);
		fscanf(f, "%c", &aux);
        fgets(linea, sizeof(linea), f);
        fscanf(f, "%c", &aux);
        inicio++;
    }

	system("cls");
    printf("\n\n\n\n\n\n\n");
    
    if(f != NULL){
        for (i=1; i<=max*2-2; i++){
			printf("                            ");
			fscanf(f, "%c", &aux);
            fgets(linea, sizeof(linea), f);
			printf("%s", linea);
        }
    }
    else{
        printf("error");
    }

	/*
	for(i=0; i <= max+1; i++){
		for(j=0)
	}*/

	fclose(f);
}

void *procesoSeparado(void *data){
    //char *texto = (char *) data;
    //int *ab = (int *) data;
    while (1){
		
		printf("juan");
		
		/*
		cond = dibujo(escCalle1, 2);

		randomUbi(&eubi);
        randomUbi(&eubi2);
        randomUbi(&eubi3);
		*/

        freeze(1);
    }    
}

void *thread_test_run (void *v) // I must not modify this function
{
    int i=1;
    while(1)
    {
       printf("into thread %d\r\n",i);
       i++; 
       sleep(1);
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !""#$%&''())*+,-./012<FPZd╚╔╩╦╠═╬¤,É¶ä¯l