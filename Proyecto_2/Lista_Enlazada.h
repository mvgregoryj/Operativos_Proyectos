#ifndef LISTAENLAZADA
#define LISTAENLAZADA

#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo{
	int Dato;
	struct Nodo *siguiente;
	struct Nodo *anterior;
}Nodo;


Nodo *CrearNodo(int dato);
int InsertarInicio(Nodo **Cabeza, int Dato);
int InsertarFinal(Nodo **Cabeza, int Dato);
int EliminarNodo(Nodo **Cabeza, int Dato);
int EliminarInicio(Nodo **Cabeza);
int EliminarFinal(Nodo **Cabeza);
void ImprimirLista(Nodo *Cabeza);
int ListaSize(Nodo **Cabeza);

#endif