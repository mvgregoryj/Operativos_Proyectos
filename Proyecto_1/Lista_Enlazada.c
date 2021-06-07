#include "Lista_Enlazada.h"



Nodo *CrearNodo(int Dato){
	Nodo *Nuevo=NULL;
	Nuevo= (Nodo*)malloc(sizeof(Nodo));
	if(Nuevo != NULL){
		Nuevo->Dato=Dato;
		Nuevo->siguiente=NULL;
		Nuevo->anterior=NULL;
	}
	return Nuevo;
}
int InsertarInicio(Nodo **Cabeza, int Dato){
	Nodo *Nuevo=NULL;
	Nuevo= CrearNodo(Dato);
	if (Nuevo!= NULL){
		Nuevo->siguiente= *Cabeza;
		Nuevo->anterior=NULL;
		if(*Cabeza!=NULL){
			(*Cabeza)->anterior=Nuevo;

		}
		*Cabeza= Nuevo;
		return 1;
	}
	return 0;
}
int EliminarInicio(Nodo **Cabeza){
	Nodo *actual=*Cabeza;
	int Numero=actual->Dato;
	if(actual->siguiente!=NULL){
		actual->siguiente->anterior=NULL;
		*Cabeza=actual->siguiente;
		free(actual);
		return Numero;
	}
	else{
		free(actual);
		*Cabeza=NULL;
		return Numero;
	}
	return -404;
}

int InsertarFinal(Nodo **Cabeza, int Dato){
	Nodo *Nuevo=NULL;
	Nodo *aux= *Cabeza;
	Nuevo=CrearNodo(Dato);
	if(Nuevo!=NULL){
		while(aux->siguiente!=NULL){
			aux=aux->siguiente;
		}
		Nuevo->anterior=aux;
		aux->siguiente=Nuevo;
		return 1;
	}
	return 0;

}
int EliminarFinal(Nodo **Cabeza){
	Nodo *aux=*Cabeza;
	int Numero;
	if(aux->siguiente!=NULL){
		while(aux->siguiente!=NULL){
			aux=aux->siguiente;
		}
		aux->anterior->siguiente=NULL;
		Numero=aux->Dato;
		free(aux);
		return Numero;
	}
	else{
		Numero=aux->Dato;
		free(aux);
		*Cabeza=NULL;
		return Numero;
	}
	return -404;
}

int EliminarNodo(Nodo **Cabeza, int Dato){
	Nodo *actual= * Cabeza , *ant=NULL, *sig=NULL;
	while(actual!=NULL){
		if(actual->Dato==Dato){
			if(actual==*Cabeza){
				*Cabeza=actual->siguiente;
				if(actual->siguiente!=NULL){
					actual->siguiente->anterior=NULL;
				}
			}
			else if(actual->siguiente==NULL){
				ant=actual->anterior;
				actual->anterior=NULL;
				ant->siguiente=NULL;
			}
			else {
				ant=actual->anterior;
				actual->anterior=NULL;
				sig=actual->siguiente;
				actual->siguiente=NULL;
				ant->siguiente=sig;
				sig->anterior=ant;
			}
			free(actual);
			return 1;

		}
		actual=actual->siguiente;
	}
	return 0;
}


void ImprimirLista(Nodo *Cabeza){
	Nodo *aux=Cabeza;
	while(aux!=NULL){
		printf("%d\n",aux->Dato );
		aux=aux->siguiente;
	}
}
