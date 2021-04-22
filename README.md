# Tp2-Algoritmos2
Trabajo Practico 2: Atrapalos a todos!

Alumno: Juan Cruz Caserío

Enunciado:
Se pide crear el TDA Pokedex y un programa que simule el uso de la misma.
La Pokedex debe contener la información de todas las especies y de los Pokémon particulares vistos por el entrenador dueño de dicho aparato.
Para esto se solicita que se haga uso de las estructuras ABB y Lista implementados con anterioridad (puede agregarprimitivas o modificar las existentes, justificando en el readme sin excepción).

Funcionamiento del programa:

-Estructuras-

En este trabajo me toco implementar el tda pokedex, en el cual su estructura (pokedex_t) esta
dada por el nombre del entrenador, una pila de los ultimos capturados, una cola de los ultimos vistos en ambos los elementos de los nodos son del tipo pokemon_t y un abb el cual los elementos
de sus nodos son del tipo especie_t que este a su vez tiene un numero que va a permitir diferenciarlo de otras especies y acomodarlo en el abb, un nombre, descripcion y una lista de pokemones cuyos elementos son del tipo pokemon_t, de los pokemones (pokemon_t) vamos a saber si fueron capturados o no, su nombre y su nivel.

-Funciones-

La idea de las funciones es:
*interactuar con archivos de texto: cargar la pokedex leyendo la informacion y poniendo a los pokemones en su correspondiente especie, evolucionar a los pokemones (en el caso de haber sido capturados) cargandolos en la nueva especie y borrandolos de la vieja, guardar la pokedex cargada en un archivo de manera tal que al ser cargado de nuevo todo quede como antes para esto utilice el iterador del arbol en recorrido preorden.

*Mostrar la informacion cargada: mostrar los ultimos capturados(y vaciar la pila),mostrar los ultimos vistos(y vaciar la cola),mostrar un pokemon en particular o mostrar todos los pokemones pertenecientes a una especie.

-Utilizar los tda abb y lista-

Gran parte del trabajo consistia en utilizar estos tdas y tener presentes las funciones implementadas(anteriormente) para utilizarlas como herramientas para el desarrollo del tp, por una necesidad aplique algunos cambios al tda lista:

Agrege la funcion lista buscar la cual recibe una lista,un elemento y un puntero posicion, busca el elemento en la lista, actualizando el valor de la posicion, y devuelve el elemento de la lista buscado. Para esto utilizo una funcion de comparacion de lista, que en la implementacion de mi trabajo compara los nombres de los pokemones y una vez que sean iguales significa que lo encontre. Otra necesidad fue el ehcho de hecho un destructor debido a que el elemento de cada nodo contiene al pokemon el cual es necesario liberar para no perder memoria, esta funcion la utilizo en el borrado y la destruccion de la lista.

-Interaccion con el usuario-
Implementacion de un programa que dado ciertos comandos haga uso de las funciones del tda pokedex, estos comandos se van 
validando o invalidando para que no surjan errores.

Linea de compilacion:

gcc comandos.c biblioteca.c pokedex.c lista.c abb.c -Wall -Werror -Wconversion -std=c99 -o comandos

Linea de ejecucion:

valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./comandos

