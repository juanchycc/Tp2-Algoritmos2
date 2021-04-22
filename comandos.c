#include"pokedex.h"
#include"colores.h"
#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>

#define ERROR -1
#define EXITO 0

typedef struct{
	char letra;
	char* descripcion;
	void (*ejecutar)(pokedex_t**);
	bool valida;
}comando_t;

char nombre[MAX_NOMBRE];


void iniciar_pokedex(pokedex_t** poke);
void guardar_pokedex(pokedex_t** poke);
void salir_pokedex(pokedex_t** poke);
void informacion_pokedex(pokedex_t** poke);
void avistar_pokedex(pokedex_t** poke);
void evolucionar_pokedex(pokedex_t** poke);
void capturados_pokedex(pokedex_t** poke);
void vistos_pokedex(pokedex_t** poke);
void mostrar_especie(pokedex_t** poke);
void mostrar_pokemon(pokedex_t** poke);

comando_t comandos[]={
	{'I',"Da inicio a la Pokedex.",iniciar_pokedex,true},
	{'G',"Guarda la Pokedex.",guardar_pokedex,false},
	{'S',"Finaliza la ejecución del programa sin guardar la información.",salir_pokedex,true},
	{'H',"Informa cuales son los comandos válidos a utilizar en cada momento.",informacion_pokedex,true},
	{'A',"Actualizará la Pokedex incorporando a ella los Pokémon avistados.",avistar_pokedex,false},
	{'E',"Actualizará la Pokedex evolucionando los Pokémon que así lo hayan hecho",evolucionar_pokedex,false},
	{'C',"Muestra los últimos Pokémon capturados.",capturados_pokedex,false},
	{'V',"Muestra los últimos Pokémon vistos.",vistos_pokedex,false},
	{'M',"Muestra la información de la especie.",mostrar_especie,false},
	{'P',"Muestra la información de un Pokémon de una determinada especie.",mostrar_pokemon,false},
	{0,NULL,NULL}
};

void iniciar_pokedex(pokedex_t** poke){
	
	*poke = pokedex_prender();
	if(!(*poke)){
		printf(COLOR_ROJO"La pokedex no pudo inicializarse\n");
		return;
	}
	strcpy((*poke)->nombre_entrenador,nombre);
	printf(COLOR_VERDE"Pokedex Inicializada con exito\n");

	for(int i=0;comandos[i].ejecutar;i++){
		if(comandos[i].letra == 'I' || comandos[i].letra == 'C' || comandos[i].letra == 'V' || comandos[i].letra == 'E')
			comandos[i].valida = false;
		else
			comandos[i].valida = true;
	}
}

void guardar_pokedex(pokedex_t** poke){
	if(pokedex_apagar(*poke) == ERROR){
		printf(COLOR_ROJO"Error al guardar la pokedex\n");
		return;
	}

	printf(COLOR_VERDE"Pokedex guardada con exito\n");
}

void salir_pokedex(pokedex_t** poke){
	printf(COLOR_VERDE"El programa se cerro\n");
	pokedex_destruir(*poke);
}

void informacion_pokedex(pokedex_t** poke){
	printf(COLOR_VERDE"Comandos validos:\n");

	for(int i=0;comandos[i].ejecutar;i++){
		if(comandos[i].valida){
			printf(COLOR_VERDE"\t%c: %s\n",comandos[i].letra,comandos[i].descripcion);
		}
	}
}

void avistar_pokedex(pokedex_t** poke){
	if(pokedex_avistar(*poke,"avistamientos.txt")==ERROR){
		printf(COLOR_ROJO"No se pudo cargar los avistados\n");
		return;
	}

	printf(COLOR_VERDE"Exito al avistar\n");

	for(int i=0;comandos[i].ejecutar;i++){
		if(comandos[i].letra == 'A')
			comandos[i].valida = false;
		else if(comandos[i].letra == 'V' || comandos[i].letra == 'C' || comandos[i].letra == 'E' )
			comandos[i].valida = true;
	}
}

void evolucionar_pokedex(pokedex_t** poke){
	if(pokedex_evolucionar(*poke,"evoluciones.txt")==ERROR){
		printf(COLOR_ROJO"No se pudo cargar los evolucionados\n");
		return;
	}

	printf(COLOR_VERDE"Exito al evolucionar\n");

	for(int i=0;comandos[i].ejecutar;i++){
		if(comandos[i].letra == 'E'){
			comandos[i].valida = false;
			return;
		}
	}
}

void capturados_pokedex(pokedex_t** poke){
	printf(COLOR_VERDE"Se muestran los ultimos calturados:\n");

	pokedex_ultimos_capturados(*poke);
}

void vistos_pokedex(pokedex_t** poke){
	printf(COLOR_VERDE"Se muestran los ultimos calturados:\n");

	pokedex_ultimos_vistos(*poke);
}

void mostrar_especie(pokedex_t** poke){
	int numero_especie;
	printf(COLOR_VERDE"Escribe el numero de la especie:\n");
	scanf("%i",&numero_especie);

	pokedex_informacion(*poke,numero_especie,"\0");
}

void mostrar_pokemon(pokedex_t** poke){
	int numero_especie;
	char nombre_pokemon[MAX_NOMBRE];
	printf(COLOR_VERDE"Escribe el numero de la especie\n");
	scanf("%i",&numero_especie);
	printf(COLOR_VERDE"Escribe el nombre del pokemon\n");
	scanf("%s",nombre_pokemon);

	pokedex_informacion(*poke,numero_especie,nombre_pokemon);

}



void ejecutar_comandos(char letra,pokedex_t** poke){
	size_t i =0;

	while( comandos[i].letra != letra && comandos[i].ejecutar)
		i++;

	if(comandos[i].letra == letra && comandos[i].valida)
		comandos[i].ejecutar(poke);
	else{
		printf(COLOR_ROJO"Escriba un comando valido\n");
		informacion_pokedex(poke);
	}

	
}

void pedir_comandos(pokedex_t* poke){
	char letra=0;
	bool finalizar = false;

	while( !finalizar){

		printf(COLOR_VERDE"Comando:");

		scanf(" %c",&letra);
		letra = (char)toupper((int)letra);
		printf("\n");

		if(letra=='H')
			ejecutar_comandos(letra,NULL);
		else
			ejecutar_comandos(letra,&poke);

		if( letra == 'S')
			finalizar = true;

		printf("\n");
	}
}

int main(){
	printf(COLOR_VERDE"\tTP 2 Atrápalos a Todos!\n\n");

	//char nombre[MAX_NOMBRE];

	printf("Entrenador! escribe tu nombre para continuar:");
	scanf("%s",nombre);
	printf("\n");

	pokedex_t* poke = NULL;

	printf(COLOR_AMARILLO"comando H para ver los comandos disponibles..\n\n");

	pedir_comandos(poke);

	return 0;
}

