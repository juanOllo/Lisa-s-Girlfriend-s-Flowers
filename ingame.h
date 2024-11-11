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
};
struct NoviaDeLisa NDL;

coor flor;
coor puertaACalle;
coor curas;

char escCalle1[max][max2];
char escCalle2[max][max2];
char escCalle3[max][max2];
char escCasa[max][max2];
char casaDeVecino[max][max2];

char escCasaLimit[max][max2];
char escCalleLimit[max][max2];

int actualLocation;

// FUNCIONES
int menuDeCasa();
int menuDeCalle();
void cargaDeEscena(char d[max][max2], int l/*line*/);
int dibujarEscena(char d[max][max2], int loc);


















// ESTA FUNCION TE VA A MOVER ENTRE DIFERENTES MENUS JUGABLES (CASA, CALLE, CASA DE VECINO)
void playGame() {

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
	/**/ entyCoor[12].x= 30;		//vecino 2
	/**/ entyCoor[12].y= 12;	
	////////////////////////////////////////////////////////////////



    int op = 0;
	do{
        // PRIMERO LLAMA AL MENU DE CASA
		op = menuDeCasa(&NDL);

		// CADA MENU DEVUELVE UN NRO QUE INDICA A Q NUEVA ESCENA VA EL PERSONAJE
		switch(op){
			case 1: {
				// menuDeCasa
				op = menuDeCasa(&NDL);
				break;
			};
			case 2: {
				// menuDeCalle
				op = menuDeCalle();
				break;
			}
			case 3: {
				// menuDeVecino
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
    //cargaDeEscena(esc);
    
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

		//	DEVUELVE 1 SI SALISTE A LA CALLE
		//	SINO DEVUELVE 0
        cond = dibujarEscena(escCasa, 1);
        
        //Unica utilizacion de int primeraVez
		if((NDL.HP < 100) && (NDL.primeraVez == 0)){
			// printf("\n[Mensaje de Lisa]:Dejé mas caramelos en tu habitacion para recuperar energia, \n\trecuerda que solo debes entregar las flores.\n\tPd.: Gracis por ayudarme tq :3\n");
			NDL.primeraVez = 1;
		}
        
        switch(cond){
            case 0:
                opc = getch();
                break;
            case 1:
				NDL.ubi.x = 11;
    			NDL.ubi.y = 6;
                return 2;
                break;
            default:
                opc = getch();
                break;
        }
        
        
	    switch(opc){
	        case 'd':
	        case 'D':
				if((escCasaLimit[max - NDL.ubi.y][NDL.ubi.x+3] != '#')){//&&(escCasaLimit[max - ubi.y][ubi.x+2] != '#')){
	            	NDL.ubi.x = NDL.ubi.x + 3;
				}
	            break;
            case 'a':
            case 'A':
				if((escCasaLimit[max - NDL.ubi.y][NDL.ubi.x-3] != '#')){//&&(escCasaLimit[max - ubi.y][ubi.x-2] != '#')){	
	            	NDL.ubi.x = NDL.ubi.x - 3;
				}
	            break;
            case 'w':
            case 'W':
				if(escCasaLimit[max - NDL.ubi.y - 1][NDL.ubi.x] != '#'){
	            	NDL.ubi.y = NDL.ubi.y + 1;
				}
	            break;
            case 's':
            case 'S':
				if(escCasaLimit[max - NDL.ubi.y + 1][NDL.ubi.x] != '#'){
	            	NDL.ubi.y = NDL.ubi.y - 1;
				}
	            break;
            default:
                break;
	    }
    }
    
}

















int menuDeCalle (){

	//system ("cls");
	int nroCasa = 1;
	int estado;
	char input;
	

	do{
		printf("\n\tESTAS EN LA CALLE:\n\n");
		printf("misionescumplidas: %d", NDL.misionesCumplidas);
			system("cls");
			actualLocation = nroCasa;
			// nroCasa = MenuCasasNuevo();
			estado = dibujarEscena(escCalle1, 2);
			printf("\nEstado: %d\n", estado);
        
        int contador001;
		
		printf("misionescumplidas: %d", NDL.misionesCumplidas);
		printf("misionescumplidas: %d", NDL.HP);
		printf("misionescumplidas: %d", NDL.cantFlores);

		switch(estado){
			case 0:
				entyCoor[1] = randomUbi(entyCoor[1]);
				entyCoor[2] = randomUbi(entyCoor[2]);
				entyCoor[3] = randomUbi(entyCoor[3]);
				break;
		    case 1:
				// ENTRAR A CASA
				return 1;
				break;
		    case 2:
				// TE ATRAPARON LAS ABEJAS, VOLVES A CASA
				return 1;
				break;
		    case 3:
				// ENTRAR A UN VECINO
			// 	return 3;
			// 	break;
		    // case 4:
		        if(actualLocation-1 > NDL.misionesCumplidas){
		            //system("cls");
		            printf("XXXX Debes entregar la flor anterior XXXX\n");
		        }
		        else{
    		        if(actualLocation <= NDL.misionesCumplidas){
                        // NDL.ubi.y = NDL.ubi.y - 1; 
    	        	    printf("\nEsta flor ya fue entregada\n");
                		if (NDL.lucides[nroCasa-1] == 1){
							printf ("(alguien entrego la flor por ti)\n");
                		}
    	        	}
    		        else{
    	        	    int AuxMisionesCumplidas = NDL.misionesCumplidas;
    	        		// menuDeCombateNuevo();
						return 3;
    	        		if(NDL.misionesCumplidas > AuxMisionesCumplidas){
    	        		    // *o = 1;
    	        		    nroCasa = 6;
    	        		}
    	        		    
    	        	}
		        }
		        break;
		    case 5: {
				system ("cls");
				if (NDL.misionesCumplidas < 4){printf("X Debes entregar todas las flores X\n");}
				else {
					// endGame(NDL);
					// *o = 7;
					nroCasa = 6;
				}
				break;
			}
		    case 6: {
				// *o = 1;
				break;
			}
			case 7: {
				//NDL.HP = 1;
				//NDL.misionesCumplidas = 0;
				NDL.cantFlores = 0;
				// *o = 3;
				nroCasa = 6;
			}
			default:
				break;
		    
		}

		input = getch();
		switch(input){
	        case 'd':
	        case 'D':
				if((escCalleLimit[max - NDL.ubi.y][NDL.ubi.x+1] != '#')&&(escCalleLimit[max - NDL.ubi.y][NDL.ubi.x+2] != '#') || (NDL.ubi.x == 59 && NDL.misionesCumplidas > 0)){
	            	NDL.ubi.x = NDL.ubi.x + 2;
				}
	            break;
            case 'a':
            case 'A':
				if((escCalleLimit[max - NDL.ubi.y][NDL.ubi.x-1] != '#')&&(escCalleLimit[max - NDL.ubi.y][NDL.ubi.x-2] != '#') || (NDL.ubi.x == 1 && actualLocation > 1)){	
	            	NDL.ubi.x = NDL.ubi.x - 2;
				}
	            break;
            case 'w':
            case 'W':
				if(escCalleLimit[max - NDL.ubi.y - 1][NDL.ubi.x] != '#'){
	            	NDL.ubi.y = NDL.ubi.y + 1;
				}
	            break;
            case 's':
            case 'S':
				if(escCalleLimit[max - NDL.ubi.y + 1][NDL.ubi.x] != '#'){
	            	NDL.ubi.y = NDL.ubi.y - 1;
				}
	            break;
            default:
                break;
	    }
		
	}while(nroCasa != 6);
}





















int dibujarEscena(char d[max][max2], int loc){
    
    // for (int i=1; i<=max; i++){
    //     for(int j=1; j<=max*2; j++){
    //         printf("%c", d[i][j]);
    //     }
    //     printf("\n");
    // }

    int j, i;
	int rAux;

    system("cls");
    printf("\n\n\n\n\n\n\n");
    switch(loc){
        case 1:											//DIBUJA LA ESCENA DE LA CASA
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

					/**/
					// if (7 == NDL.ubi.x && 1 == NDL.ubi.y){
					// 	endGame(&NDL);
					// }
					/**/
                }
                
                printf("\n");
            }

			if(proximo(NDL.ubi, puertaACalle) == 1){
				return 1;
            }
            if(proximo(NDL.ubi, flor) == 1 && NDL.cantFlores == 0){
				NDL.cantFlores = 1;
				printf("\nTomaste una flor.");
            }
            if(proximo(NDL.ubi, curas) == 1 && NDL.HP < 100){
				NDL.HP = 100;
				printf("\nTomaste una de las pastillas.");
            }
            
			printf("\n\n\n\t\t\t\t\t   <3 HP: %d <3", NDL.HP);
        	printf("\tFLOR EN MANO: %d \n\n", NDL.cantFlores);

            break;
		case 2:											//DIBUJA LA ESCENA DE LA CALLE
            for(i = max-1; i > 0; i--){
                printf("                              ");
                for(j = 1; j < max2; j++){
                    if((actualLocation == NDL.misionesCumplidas) && (NDL.misionesCumplidas > 0) && ((j == entyCoor[1].x) && (i == entyCoor[1].y) || (j == entyCoor[2].x) && (i == entyCoor[2].y) || (j == entyCoor[3].x) && (i == entyCoor[3].y))){
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
            if(proximo(NDL.ubi, entyCoor[10])==2 && actualLocation==1){
				NDL.ubi.x = 52;
    			NDL.ubi.y = 6;
                return 1;
				// agregar ubi de NDL entrando a casa
            }

			// ATRAPADA POR ABEJA
            if(((actualLocation == NDL.misionesCumplidas)) && (NDL.cantFlores > 0 && NDL.misionesCumplidas > 0) && ((proximo(NDL.ubi, entyCoor[1]) > 0) || (proximo(NDL.ubi, entyCoor[2]) > 0) || (proximo(NDL.ubi, entyCoor[3]) > 0))){
				NDL.cantFlores = 0;
				NDL.ubi.x = 31;
    			NDL.ubi.y = 6;
                return 2;
            }

			// ENTRAR A VECINO 1 O 2
            if((proximo(NDL.ubi, entyCoor[11]) == 2 && actualLocation==1)
					||
				(proximo(NDL.ubi, entyCoor[12]) == 2 && actualLocation==2)
			){
                return 3;	
            }

            
			// 		ESTO ES PARA PASAR DE CALLE
			// if(actualLocation == 1 && NDL.ubi.x > 59){
			// 	return 2;
			// }
			// if(actualLocation == 2 && NDL.ubi.x < 1){
			// 	// return 1;
			// }

            break;

        default:
            break;
    }
    return 0;
}