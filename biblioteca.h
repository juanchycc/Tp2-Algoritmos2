#ifndef _BIBLIOTECA_H
#define __BIBLIOTECA_H__

#include<stdio.h>
#include<string.h>

#define ERROR_MEMORIA NULL
#define ERROR -1
#define EXITO 0
#define IGUALES 0
#define PRIMERO_MAYOR 1
#define PRIMERO_MENOR -1
#define ESPECIE 'E'

int arbol_comparador(void* primera_especie,void* segunda_especie);



void arbol_destructor(void* especie);



int lista_comparador(void* pokemon,void* nombre_buscado);



void lista_destructor(void* pokemon);



/*
Elimina el pokemon de la especie vieja y lo inserta en la especie a la cual evolucciono,
si la especie evolucionada no existe la crea e inicializa.
*/
int insertar_evolucionado(int numero_especie_evolucionada,especie_pokemon_t especie_aux,particular_pokemon_t pokemon_evolucionado,pokedex_t* pokedex);



/*
Crea un pokemon y le carga los datos del pokemon recibido, si la especie
correspondiente existe lo inserta, sino, crea la especie e inserta el pokemon.
*/
int insertar_avistados(especie_pokemon_t especie_aux,particular_pokemon_t pokemon_avistado,char capturado,pokedex_t* pokedex);



/*
Si recibe un poke valido imprime los datos de este.
*/
void imprimir_pokemon(void* pokemon,void* contexto);


/*
Imprime los ultimos vistos y la deja vacia
*/
void mostrar_vistos(lista_t* vistos);



/*
Imprime los ultimos capturados y la deja vacia
*/
void mostrar_capturados(lista_t* capturados);



/*
Funcion para iterador de arbol, escribe la informacion de la especie
recibida en el archivo y llama al iterador para guardar sus pokemones
*/
bool escribir_guardado(void* especie, void* pokedex_guardada);




/*
Intenta crear e inicializar una especie copiando mediante una lectura
los datos del archivo recibido.
*/
especie_pokemon_t* especie_prender(FILE* pokedex_guardada);



/*
Intenta crear e inicializar un pokemon copiando mediante una lectura
los datos del archivo recibido.
*/
particular_pokemon_t* pokemon_prender(FILE* pokedex_guardada);


#endif /* __BIBLIOTECA_H__ */