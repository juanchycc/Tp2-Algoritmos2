#include"pokedex.h"
#include<stdbool.h>
#include<stdio.h>
#include<string.h>

#define ERROR_MEMORIA NULL
#define ERROR -1
#define EXITO 0
#define SI 'S'
#define NO 'N'
#define IGUALES 0
#define PRIMERO_MAYOR 1
#define PRIMERO_MENOR -1
#define FORMATO_ESPECIE_GUARDADA "%c;%i;%s;%s\n"
#define FORMATO_POKEMON_GUARDADO "%c;%s;%i;%c\n"
#define FORMATO_ESPECIE_PRENDER "%i;%[^;];%[^\n]\n"
#define FORMATO_POKEMON_PRENDER "%[^;];%i;%c\n"
#define ESPECIE 'E'
#define POKEMON 'P'


int arbol_comparador(void* primera_especie,void* segunda_especie){
    
    if(((especie_pokemon_t*)primera_especie)->numero > (((especie_pokemon_t*)segunda_especie)->numero))
        return PRIMERO_MAYOR;
    else if(((especie_pokemon_t*)primera_especie)->numero < (((especie_pokemon_t*)segunda_especie)->numero))
        return PRIMERO_MENOR;
    else
        return IGUALES;
}

void arbol_destructor(void* especie){
    lista_destruir(((especie_pokemon_t*)especie)->pokemones);
    free(especie);
}

int lista_comparador(void* pokemon,void* nombre_buscado){
    return strcmp(((particular_pokemon_t*)pokemon)->nombre,(char*)nombre_buscado);
}
void lista_destructor(void* pokemon){
    free(pokemon);
}



/*
Intenta obtener una direccion con la memoria requerida para crear una especie,
recibe una especie cargada cuyos datos se copian en la direccion de memoria de la especie
creada y un pokemon que se inserta en la lista de pokemones de dicha especie.
 */
especie_pokemon_t* crear_especie(especie_pokemon_t especie_aux,particular_pokemon_t* pokemon){
    especie_pokemon_t* especie_nueva = calloc(1,sizeof(especie_pokemon_t));
    if(!especie_nueva)
        return NULL;
    especie_nueva->pokemones = lista_crear(&lista_comparador,&lista_destructor);
    if(!especie_nueva->pokemones){
        free(especie_nueva);
        return NULL;
    }
    especie_nueva->numero = especie_aux.numero;
    strcpy(especie_nueva->nombre,especie_aux.nombre);
    strcpy(especie_nueva->descripcion,especie_aux.descripcion);
    lista_insertar(especie_nueva->pokemones,pokemon);
    return especie_nueva;
}



/*
Elimina el pokemon de la especie vieja y lo inserta en la especie a la cual evolucciono,
si la especie evolucionada no existe la crea e inicializa.
*/
int insertar_evolucionado(int numero_especie_evolucionada,especie_pokemon_t especie_aux,particular_pokemon_t pokemon,pokedex_t* pokedex){
    especie_pokemon_t* especie_vieja = arbol_buscar(pokedex->pokemones,&especie_aux);
    if( !especie_vieja)
        return ERROR;

    especie_aux.numero = numero_especie_evolucionada;
    especie_pokemon_t* especie_evolucionada = arbol_buscar(pokedex->pokemones,&especie_aux);

    size_t posicion = 0;
    particular_pokemon_t* pokemon_evolucionado = calloc(1,sizeof(particular_pokemon_t));
    if( !pokemon_evolucionado )
        return ERROR;

    particular_pokemon_t* pokemon_buscado = lista_buscar(especie_vieja->pokemones,pokemon.nombre,&posicion);
    if( !pokemon_buscado ){
        free(pokemon_evolucionado);
        return EXITO; //el pokemon no exite, pero no surgio ningun error;
    }

    
    memcpy(pokemon_evolucionado,pokemon_buscado,sizeof(particular_pokemon_t));
    if(!pokemon_evolucionado){
        free(pokemon_evolucionado);
        return ERROR;
    }
    
   

    if( !pokemon_evolucionado->capturado ){
        free(pokemon_evolucionado);
        return EXITO; //no surgio un error, el pokemon no fue capturado por lo tanto no evoluciona
    }
    

    if( !especie_evolucionada ){
        especie_evolucionada = crear_especie(especie_aux,pokemon_evolucionado);
        if(!especie_evolucionada){
            free(pokemon_evolucionado);
            return ERROR;
        }
        
        arbol_insertar(pokedex->pokemones,especie_evolucionada);

    }else
        lista_insertar(especie_evolucionada->pokemones,pokemon_evolucionado);
    
    lista_borrar_de_posicion(especie_vieja->pokemones,posicion);
    
    return EXITO;
}



/*
Intenta obtener la direccion de memoria necesaria para un nuevo pokemon,
si la consiguio le carga la informacion del pokemon recibido y si este fue
capturado lo apila sino lo encola.
*/
void actualizar_pokedex(lista_t* pila,lista_t* cola,particular_pokemon_t* pokemon){
    
    particular_pokemon_t* pokemon_actualizado = calloc(1,sizeof(particular_pokemon_t));
    if(!pokemon_actualizado)
        return;

    memcpy(pokemon_actualizado,pokemon,sizeof(particular_pokemon_t));

    if( pokemon_actualizado->capturado)
        lista_apilar(pila,pokemon_actualizado);
    else
        lista_encolar(cola,pokemon_actualizado);
}



/*
Crea un pokemon y le carga los datos del pokemon recibido, si la especie
correspondiente existe lo inserta, sino, crea la especie e inserta el pokemon.
*/
int insertar_avistados(especie_pokemon_t especie_aux,particular_pokemon_t pokemon_avistado,char capturado,pokedex_t* pokedex){
    if( capturado == SI)
        pokemon_avistado.capturado = true;
    else
        pokemon_avistado.capturado = false;

    particular_pokemon_t* pokemon = calloc(1,sizeof(particular_pokemon_t));
    if(!pokemon)
        return ERROR;

    *pokemon = pokemon_avistado;

    especie_pokemon_t* especie_avistada = arbol_buscar(pokedex->pokemones,&especie_aux);


    if(!especie_avistada){
        especie_avistada = crear_especie(especie_aux,pokemon);
        if(!especie_avistada){
            free(pokemon);
            return ERROR;
        }
        arbol_insertar(pokedex->pokemones,especie_avistada);

    }else
        lista_insertar(especie_avistada->pokemones,pokemon);

    actualizar_pokedex(pokedex->ultimos_capturados,pokedex->ultimos_vistos,pokemon);
    return EXITO;
}



/*
Si recibe un poke valido imprime los datos de este.
*/
void imprimir_pokemon(void* pokemon,void* contexto){
    if(!pokemon){
        printf("No hay pokemon para mostrar\n");
        return;
    }

    printf("\tNombre:%s\n",((particular_pokemon_t*)pokemon)->nombre);
    printf("\t\tNivel:%i\n",((particular_pokemon_t*)pokemon)->nivel);
    if( ((particular_pokemon_t*)pokemon)->capturado)
        printf("\t\tCapturado:SI\n");
    else
        printf("\t\tCapturado:NO\n");
}



/*
Imprime los ultimos vistos y la deja vacia
*/
void mostrar_vistos(lista_t* vistos){
    if( lista_vacia(vistos))
        return;

    particular_pokemon_t* pokemon_visto = lista_elemento_en_posicion(vistos,0);
    printf("\tNombre del pokemon:%s\n",pokemon_visto->nombre);
    printf("\tNivel del pokemon:%i\n",pokemon_visto->nivel);

    lista_desencolar(vistos);
    mostrar_vistos(vistos);
}



/*
Imprime los ultimos capturados y la deja vacia
*/
void mostrar_capturados(lista_t* capturados){
    if( lista_vacia(capturados))
        return;

    particular_pokemon_t* pokemon_capturado = lista_ultimo(capturados);
    printf("\tNombre del pokemon:%s\n",pokemon_capturado->nombre);
    printf("\tNivel del pokemon:%i\n",pokemon_capturado->nivel);

    lista_desapilar(capturados);
    mostrar_capturados(capturados);
}


/*
Funcion para iterador de lista, escribe la informacion del pokemon
recibido en el archivo.
*/
void guardar_pokemones(void* pokemon,void* pokedex_guardada){

    char capturado = NO;
    if( ((particular_pokemon_t*)pokemon)->capturado)
        capturado = SI;

    fprintf((FILE*)pokedex_guardada,FORMATO_POKEMON_GUARDADO,POKEMON,((particular_pokemon_t*)pokemon)->nombre,((particular_pokemon_t*)pokemon)->nivel,capturado);

}


/*
Funcion para iterador de arbol, escribe la informacion de la especie
recibida en el archivo y llama al iterador para guardar sus pokemones
*/
bool escribir_guardado(void* especie, void* pokedex_guardada){
    if( !especie )
        return true;

    fprintf((FILE*)pokedex_guardada,FORMATO_ESPECIE_GUARDADA,ESPECIE,((especie_pokemon_t*)especie)->numero,((especie_pokemon_t*)especie)->nombre,((especie_pokemon_t*)especie)->descripcion);

    if(!lista_vacia(((especie_pokemon_t*)especie)->pokemones))
        lista_con_cada_elemento(((especie_pokemon_t*)especie)->pokemones,guardar_pokemones,(FILE*)pokedex_guardada);

    return false;
}



/*
Intenta crear e inicializar una especie copiando mediante una lectura
los datos del archivo recibido.
*/
especie_pokemon_t* especie_prender(FILE* pokedex_guardada){
    especie_pokemon_t* especie = calloc(1,sizeof(especie_pokemon_t));
    if(!especie)
        return ERROR_MEMORIA;
       
    especie->pokemones = lista_crear(&lista_comparador,&lista_destructor);
    if(!especie->pokemones){
        free(especie);
        return ERROR_MEMORIA;
    }

    fscanf(pokedex_guardada,FORMATO_ESPECIE_PRENDER,&(especie->numero),especie->nombre,especie->descripcion);
    return especie;
}



/*
Intenta crear e inicializar un pokemon copiando mediante una lectura
los datos del archivo recibido.
*/
particular_pokemon_t* pokemon_prender(FILE* pokedex_guardada){
    particular_pokemon_t* pokemon = calloc(1,sizeof(particular_pokemon_t));
    if(!pokemon)
        return ERROR_MEMORIA;

    char capturado = NO;

    fscanf(pokedex_guardada,FORMATO_POKEMON_PRENDER,pokemon->nombre,&(pokemon->nivel),&capturado);
    
    if( capturado == SI)
        pokemon->capturado = true;
    else
        pokemon->capturado = false;

    return pokemon;
}

