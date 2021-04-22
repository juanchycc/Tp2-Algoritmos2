#include"pokedex.h"
#include"biblioteca.h"

#define CANTIDAD_LEIDA 5
#define FORMATO_AVISTADOS "%i;%[^;];%[^;];%[^;];%i;%c\n"
#define FORMATO_EVOLUCIONADOS "%i;%[^;];%i;%[^;];%[^\n]\n"
#define RUTA_POKEDEX "pokedex.txt"


pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){

    pokedex_t* pokedex = calloc(1,sizeof(pokedex_t));
    if( !pokedex )
        return ERROR_MEMORIA;
    
    pokedex->ultimos_capturados = lista_crear(lista_comparador,lista_destructor);
    if( !(pokedex->ultimos_capturados) ){
        free(pokedex);
        return ERROR_MEMORIA;
    }

    pokedex->ultimos_vistos = lista_crear(lista_comparador,lista_destructor);
    if( !(pokedex->ultimos_vistos) ){
        free(pokedex->ultimos_capturados);
        free(pokedex);
        return ERROR_MEMORIA;
    }

    pokedex->pokemones = arbol_crear(&(arbol_comparador),&(arbol_destructor));
    if( !(pokedex->pokemones)){
        free(pokedex->ultimos_capturados);
        free(pokedex->ultimos_vistos);
        free(pokedex);
        return ERROR_MEMORIA; 
    }

    strcpy( pokedex->nombre_entrenador,entrenador );
    return pokedex;
}


int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if( !pokedex || !ruta_archivo)
        return ERROR;

    FILE* evolucionados = fopen(ruta_archivo,"r");

    int numero_especie_evolucionada;
    especie_pokemon_t especie_aux;
    particular_pokemon_t pokemon_evolucionado;

    int leidos = fscanf(evolucionados,FORMATO_EVOLUCIONADOS,&(especie_aux.numero),pokemon_evolucionado.nombre,&(numero_especie_evolucionada),especie_aux.nombre,especie_aux.descripcion);
    
    int inserto = insertar_evolucionado(numero_especie_evolucionada,especie_aux,pokemon_evolucionado,pokedex);

    while( leidos == CANTIDAD_LEIDA && inserto == EXITO){
        leidos = fscanf(evolucionados,FORMATO_EVOLUCIONADOS,&(especie_aux.numero),pokemon_evolucionado.nombre,&(numero_especie_evolucionada),especie_aux.nombre,especie_aux.descripcion);
        if(leidos == CANTIDAD_LEIDA){
            inserto = insertar_evolucionado(numero_especie_evolucionada,especie_aux,pokemon_evolucionado,pokedex);
        }
    }

    fclose(evolucionados);
    return inserto;
}

int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if( !pokedex || !ruta_archivo )
        return ERROR;
    FILE* avistados = fopen(ruta_archivo,"r");

    especie_pokemon_t especie_aux;
    particular_pokemon_t pokemon_avistado;
    char capturado;

    int leidos = fscanf(avistados,FORMATO_AVISTADOS,&(especie_aux.numero),especie_aux.nombre,especie_aux.descripcion,pokemon_avistado.nombre,&(pokemon_avistado.nivel),&capturado);

    int inserto = insertar_avistados(especie_aux,pokemon_avistado,capturado,pokedex);

    while(leidos!=EOF && inserto==EXITO){
        leidos = fscanf(avistados,FORMATO_AVISTADOS,&(especie_aux.numero),especie_aux.nombre,especie_aux.descripcion,pokemon_avistado.nombre,&(pokemon_avistado.nivel),&capturado);
        if(leidos != EOF){
            inserto = insertar_avistados(especie_aux,pokemon_avistado,capturado,pokedex);
        }
    }
    
    fclose(avistados);
    return inserto;
}



void pokedex_ultimos_capturados(pokedex_t* pokedex){
    if( !pokedex)
        return;
    printf("Ultimos Capturados:\n");
    mostrar_capturados(pokedex->ultimos_capturados);
}




void pokedex_ultimos_vistos(pokedex_t* pokedex){
    if(!pokedex)
        return;
    printf("Ultimos Vistos:\n");
    mostrar_vistos(pokedex->ultimos_vistos);
}



void pokedex_informacion(pokedex_t* pokedex, int numero_pokemon, char nombre_pokemon[MAX_NOMBRE]){
    if( !pokedex )
        return;

    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones,&numero_pokemon);

    if(!especie){
        printf("La especie buscada es desconocida, intente de nuevo al capturarla\n");
        return;
    }

    if( strlen(nombre_pokemon)==0){
        printf("Se imprimen todos los pokemones de la especie:%s\n",especie->nombre);
        lista_con_cada_elemento(especie->pokemones,imprimir_pokemon,0);
    }else{
        size_t posicion = 0;
        particular_pokemon_t* pokemon = lista_buscar(especie->pokemones,nombre_pokemon,&posicion);
        if(!pokemon){
            printf("El pokemon buscado es desconocido, intente de nuevo al capturarlo\n");
            return;
        }
        
        printf("Se imprime la informacion de un pokemon:\n");
        imprimir_pokemon(pokemon,0);
    }
}

void pokedex_destruir(pokedex_t* pokedex){
    if(!pokedex)
        return;

    arbol_destruir(pokedex->pokemones);
    lista_destruir(pokedex->ultimos_vistos);
    lista_destruir(pokedex->ultimos_capturados);
    free(pokedex);
}

int pokedex_apagar(pokedex_t* pokedex){
    if( !pokedex )
        return ERROR;

    FILE* pokedex_guardada = fopen(RUTA_POKEDEX,"w");

    fprintf(pokedex_guardada,"%s\n",pokedex->nombre_entrenador);

    if( !arbol_vacio(pokedex->pokemones))
        abb_con_cada_elemento(pokedex->pokemones, ABB_RECORRER_PREORDEN,escribir_guardado,pokedex_guardada);

    fclose(pokedex_guardada);
    return EXITO;
}


pokedex_t* pokedex_prender(){

    FILE* pokedex_guardada = fopen(RUTA_POKEDEX,"r");
    if( !pokedex_guardada)
        return ERROR_MEMORIA;

    char nombre[MAX_NOMBRE];
    strcpy(nombre,"\0");
    fscanf(pokedex_guardada,"%[^\n]\n",nombre);

    pokedex_t* pokedex = pokedex_crear(nombre);
    if( !pokedex)
        return ERROR_MEMORIA;

    char tipo;
    especie_pokemon_t* especie_actual = NULL;

    while(fscanf(pokedex_guardada,"%c;",&tipo) == 1){

        if( tipo == ESPECIE){
           especie_actual = especie_prender(pokedex_guardada);
            arbol_insertar(pokedex->pokemones,especie_actual);
        }else{
            if(especie_actual){
                particular_pokemon_t* pokemon_actual = NULL;
                pokemon_actual = pokemon_prender(pokedex_guardada);
                if(!pokemon_actual)
                    return ERROR_MEMORIA;

                lista_insertar(especie_actual->pokemones,pokemon_actual);
            }
        }

    }

    fclose(pokedex_guardada);
    return pokedex;
}







