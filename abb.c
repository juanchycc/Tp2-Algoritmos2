#include "abb.h"

#define EXITO 0
#define ERROR -1
#define ERROR_RECORRIDO 0
#define PRIMERO_MAYOR 1
#define PRIMERO_MENOR -1
#define AMBOS_IGUALES 0

nodo_abb_t* crear_nodo_arbol(){

	nodo_abb_t* nodo = calloc(1,sizeof(nodo_abb_t));

	if(!nodo)
		return NULL;

	return nodo;
}

int crear_raiz(abb_t* arbol,void* elemento){
	
	arbol->nodo_raiz = crear_nodo_arbol();

	if( arbol->nodo_raiz )
		arbol->nodo_raiz->elemento = elemento;
	else
		return ERROR;

	return EXITO;
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	abb_t* arbol = malloc(sizeof(abb_t));

	if( !arbol )
		return NULL;

	arbol->comparador = comparador;
	arbol->destructor = destructor;
	arbol->nodo_raiz = NULL;

	return arbol;
}

nodo_abb_t* insertar( nodo_abb_t* nodo, void*  elemento,int (comparador)(void*,void*)){
	if( nodo == NULL ){
		nodo = crear_nodo_arbol();
		nodo->elemento = elemento;
		if( !nodo )
			return NULL;
		return nodo;
	}

	int comparacion = comparador(elemento,nodo->elemento);

	if( comparacion==PRIMERO_MENOR )
		nodo->izquierda = insertar(nodo->izquierda,elemento,comparador);

	else if( comparacion==PRIMERO_MAYOR )
			nodo->derecha = insertar(nodo->derecha,elemento,comparador);

	else{
		nodo_abb_t* nodo_nuevo = crear_nodo_arbol();
		nodo_nuevo->elemento = elemento;

		if( !nodo_nuevo )
			return nodo;

		nodo_nuevo->derecha = nodo->derecha;
		nodo->derecha = nodo_nuevo;
	}

	return nodo; 
}

int arbol_insertar(abb_t* arbol, void* elemento){
 	if(!arbol)
 		return ERROR;

 	if(!arbol->nodo_raiz)
 		return crear_raiz(arbol,elemento);
 	
 	insertar(arbol->nodo_raiz,elemento,arbol->comparador);

 	if(!arbol_buscar(arbol,elemento))
 		return ERROR;
 	else
 		return EXITO;
 }

nodo_abb_t* buscar_mayor_de_menores(nodo_abb_t* nodo){
	if(!nodo)
		return NULL;

	nodo_abb_t* aux = nodo->izquierda;
	bool encontrado = false;

	while(aux != NULL && !encontrado){

		if( aux->derecha == NULL)
			encontrado = true;
		else
			aux = aux->derecha;
	}
	if(encontrado)
		return aux;
	else
		return NULL;
}

nodo_abb_t* eliminar(nodo_abb_t* nodo,void* elemento,int (comparador)(void*,void*),void (destructor)(void*)){
	if(!nodo)
		return NULL;

	nodo_abb_t* reemplazo;
	int comparacion = comparador(elemento,nodo->elemento);

	if( comparacion == PRIMERO_MENOR )
		nodo->izquierda = eliminar(nodo->izquierda,elemento,comparador,destructor);
	
	else if( comparacion == PRIMERO_MAYOR )
		nodo->derecha = eliminar(nodo->derecha,elemento,comparador,destructor);
	
	else if( nodo->izquierda && nodo->derecha ){
		
		void* auxiliar;

		reemplazo = buscar_mayor_de_menores(nodo);

		auxiliar = nodo->elemento;
		nodo->elemento = reemplazo->elemento;
		reemplazo->elemento =  auxiliar;

		nodo->izquierda = eliminar(nodo->izquierda,reemplazo->elemento,comparador,destructor);

	}else{
		reemplazo = nodo;
		nodo_abb_t* hijo;

		if( nodo->izquierda == NULL )
			hijo = nodo->derecha;
		else if( nodo->derecha == NULL )
			hijo = nodo->izquierda;
		else
			hijo = NULL;
		
		destructor(reemplazo->elemento);
		free( reemplazo );

		return hijo;
		
	}

	return nodo;
}

int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol)
		return ERROR;
	arbol->nodo_raiz = eliminar(arbol->nodo_raiz,elemento,arbol->comparador,arbol->destructor);

	if(!arbol_buscar(arbol,elemento))
		return EXITO;
	else
		return ERROR;
}

nodo_abb_t* buscar(nodo_abb_t* nodo,void* elemento,int (comparador)(void*,void*)){
	if(!nodo)
		return NULL;
 	
	int comparacion = comparador(elemento,nodo->elemento);

	if( comparacion == PRIMERO_MENOR)
		return buscar(nodo->izquierda,elemento,comparador);

	else if( comparacion == PRIMERO_MAYOR)
		return buscar(nodo->derecha,elemento,comparador);

	else
		return nodo->elemento;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(!arbol)
		return NULL;

	return buscar(arbol->nodo_raiz,elemento,arbol->comparador);
}

void* arbol_raiz(abb_t* arbol){
	if(!arbol || !arbol->nodo_raiz)
		return NULL;

	return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
	if(!arbol || !arbol->nodo_raiz)
		return true;

	return false;
}

int inorden_recursivo(nodo_abb_t* nodo, void** array, int tamanio_array,int cantidad){

	if( nodo->izquierda )
		cantidad = inorden_recursivo(nodo->izquierda,array,tamanio_array,cantidad);
	
	if( cantidad < tamanio_array){
		array[cantidad] = nodo->elemento;
		cantidad++;
	}

	if(cantidad >= tamanio_array)
		return cantidad;
	

	if( nodo->derecha )
		cantidad = inorden_recursivo(nodo->derecha,array,tamanio_array,cantidad);
	
	return cantidad;
}
int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !array)
		return ERROR_RECORRIDO;

	int cantidad = 0;
	
	return inorden_recursivo(arbol->nodo_raiz, array, tamanio_array,cantidad);
}

int preorden_resursivo(nodo_abb_t* nodo, void** array, int tamanio_array,int cantidad){

	if( cantidad < tamanio_array){

		array[cantidad] = nodo->elemento;
		cantidad++;
	}

	if(cantidad >= tamanio_array)
		return cantidad;
	
	if( nodo->izquierda )
		cantidad = preorden_resursivo(nodo->izquierda,array,tamanio_array,cantidad);
		

	if( nodo->derecha )
		cantidad = preorden_resursivo(nodo->derecha,array,tamanio_array,cantidad);
		
	return cantidad;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !array)
		return ERROR_RECORRIDO;

	int cantidad = 0;
	
	return preorden_resursivo(arbol->nodo_raiz, array, tamanio_array,cantidad);
}

int postorden_recursivo(nodo_abb_t* nodo, void** array, int tamanio_array,int cantidad){

	if( nodo->izquierda )
		cantidad = postorden_recursivo(nodo->izquierda,array,tamanio_array,cantidad);
		
	if( nodo->derecha )
		cantidad = postorden_recursivo(nodo->derecha,array,tamanio_array,cantidad);

	if( cantidad < tamanio_array){
		array[cantidad] = nodo->elemento;
		cantidad++;
	}
	
	return cantidad;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !array)
		return ERROR_RECORRIDO;

	int cantidad = 0;
	
	return postorden_recursivo(arbol->nodo_raiz, array, tamanio_array,cantidad);
}
void destruir_nodo(nodo_abb_t* nodo,void (destructor)(void*)){
	
	if(nodo->izquierda)
		destruir_nodo(nodo->izquierda,destructor);
	if(nodo->derecha)
		destruir_nodo(nodo->derecha,destructor);

	destructor(nodo->elemento);
	free(nodo);
}
void arbol_destruir(abb_t* arbol){
	
	if(!arbol)
		return;
	if( arbol->nodo_raiz )
		destruir_nodo(arbol->nodo_raiz,arbol->destructor);

	free(arbol);
}

bool con_cada_elemento_inorden(nodo_abb_t* nodo,bool (funcion)(void*,void*),void* extra){

	if( nodo->izquierda ){
		if(con_cada_elemento_inorden(nodo->izquierda,funcion,extra)  == true)
			return true;
	}
	
	if(funcion(nodo->elemento,extra)==true)
		return true;

	if( nodo->derecha ){
		if(con_cada_elemento_inorden(nodo->derecha,funcion,extra)==true)
			return true;
	}

	return false;	
}

bool con_cada_elemento_preorden(nodo_abb_t* nodo,bool (funcion)(void*,void*),void* extra){

	if(funcion(nodo->elemento,extra)==true)
		return true;

	if( nodo->izquierda ){
		if(con_cada_elemento_preorden(nodo->izquierda,funcion,extra)==true)
			return true;
	}

	if( nodo->derecha ){
		if(con_cada_elemento_preorden(nodo->derecha,funcion,extra)==true)
			return true;	
	}

	return false;
}

bool con_cada_elemento_postorden(nodo_abb_t* nodo,bool (funcion)(void*,void*),void* extra){

	if( nodo->izquierda ){
		if(con_cada_elemento_postorden(nodo->izquierda,funcion,extra)==true)
			return true;
	}

	if( nodo->derecha ){
		if(con_cada_elemento_postorden(nodo->derecha,funcion,extra)==true)
			return true;
	}

	if(funcion(nodo->elemento,extra)==true)
		return true;

	return false;
}


void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(!arbol || !funcion)
		return;
	
	if(recorrido == ABB_RECORRER_INORDEN)
		con_cada_elemento_inorden(arbol->nodo_raiz,funcion,extra);
	else if( recorrido == ABB_RECORRER_PREORDEN)
		con_cada_elemento_preorden(arbol->nodo_raiz,funcion,extra);
	else
		con_cada_elemento_postorden(arbol->nodo_raiz,funcion,extra);
		
}

