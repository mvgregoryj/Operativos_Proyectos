/**
 * @file primosn.c
 * @author 	Jonathan Bautista 16-10109
 * 		   		Daniela Ramirez 16-10940
 * 		   		Gregory Mu;oz 16-11313
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "Lista_Enlazada.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


void ProgramError(long x );
void *Crear_hilos(void *arg);
void *ComprobarNumeroPrimoHilo(void *arg);
int Tellme_lines(char *Archivo);
int esPrimo(int numero);

struct Parametros{
	int Numero_Hilos;
	int NumeroLineas;
	Nodo *Lista_Numeros;
};

struct Parametros_{
	int Hilo;
	int Tarea;
	int Hilos;
	Nodo *Lista_N;
	Nodo *Lista_Primos;
};


int main(int argc, char *argv[]){
	char ca;
	int cuenta=0;
	cuenta=Tellme_lines(argv[1]);
	int Numero=0;
	int i=0;
	int a[1]={0};
	long Num_threads;
	Nodo *Lista_Numeros=NULL;
	Num_threads=strtol(argv[2],NULL,10);  // Numero de hilos solicitados por consola 
	ProgramError(Num_threads);
	FILE *file=fopen(argv[1], "rt");
	if (file==NULL){
		perror("Error en la apertura del archivo.");
		return 1;
	}
	while(!feof(file)&& i!=cuenta){
		if(i==0){
			fscanf(file,"%d",&Numero); // extrae del archivo el numero de la linea.
			InsertarInicio(&Lista_Numeros, Numero); // inserta en la lista enlazada el numero respectivo de cada linea
			i=i+1;  //El número de lineas
		}
		fscanf(file,"%d",&Numero);
		InsertarFinal(&Lista_Numeros, Numero);
		i=i+1;  //El número de lineas

	}


	pthread_t Master;
	struct Parametros p;
	p.Numero_Hilos=Num_threads;
	p.Lista_Numeros=Lista_Numeros;
	p.NumeroLineas=cuenta;
	pthread_create(&Master,NULL, Crear_hilos,(void *)&p);
	pthread_join(Master,NULL);

}


void *Crear_hilos(void *arg){
	struct Parametros *p;
	p=(struct Parametros *)arg;
	struct Parametros_ p1;
	int Tarea1=(p->NumeroLineas)/(p->Numero_Hilos);
	int Tarea2=(p->NumeroLineas)/(p->Numero_Hilos)+(p->NumeroLineas)%(p->Numero_Hilos);
	p1.Lista_N=p->Lista_Numeros;
	p1.Hilos=p->Numero_Hilos;
	Nodo *Cantidad_Primos=NULL;
	p1.Lista_Primos=Cantidad_Primos;
	pthread_t Worker;
	for(int i=0;i<p->Numero_Hilos;i++){
		if(i!=(p->Numero_Hilos-1)){
			p1.Hilo=i;
			p1.Tarea=Tarea1;
			pthread_create(&Worker,NULL,ComprobarNumeroPrimoHilo,(void *)&p1);
			pthread_join(Worker,NULL);
		}
		else if(i==(p->Numero_Hilos-1)){
			p1.Hilo=i;
			p1.Tarea=Tarea2;
			pthread_create(&Worker,NULL,ComprobarNumeroPrimoHilo,(void *)&p1);
			pthread_join(Worker,NULL);

		}
		
	}
	int Cantidad=0;
	Cantidad=Tellme_lines("Primos.txt");
	printf("La cantidad de numeros primos es: %d\n", Cantidad);
	remove("Primos.txt");
	
}


void *ComprobarNumeroPrimoHilo(void *arg){
	struct Parametros_ *s;
	s=(struct Parametros_ *)arg;
	int Chequear_Numero;
	int i;
	i=s->Hilo;

	for(int j=0;j<s->Tarea;j++){
		Chequear_Numero=EliminarInicio(&(s->Lista_N));
		if(esPrimo(Chequear_Numero)==1){
			InsertarInicio(&(s->Lista_Primos),Chequear_Numero);
		}
	}
	if(i==s->Hilos-1){
		int Numero_Primos=0;
		Numero_Primos=ListaSize(&s->Lista_Primos);
		/* Numero_Primos++; */
		char texto[]="Primos.txt";
		FILE *file=fopen(texto,"w");
		if(file==NULL){
		perror("Error en la creacion del archivo \n\n");

		}else{

		for(int j=0;j<Numero_Primos;j++){
			Chequear_Numero= EliminarInicio(&(s->Lista_Primos));
			fprintf(file, "%d", Chequear_Numero);
			fputc('\n',file) ;
			}
			fflush(file);
			fclose(file);
		}
		
	}

}


int esPrimo(int numero) {
  if (numero == 0 || numero == 1){

  	return 0;
  }
  /*
          El número 4 es un caso especial, pues al dividirlo entre
          2 el resultado es 2, y el ciclo nunca se cumple, indicando que
          el 4 SÍ es primo, pero realmente NO lo es, así que si el número es 4
                        inmediatamente indicamos que no es primo (regresando 0)
          Nota: solo es para el 4, los demás funcionan bien
  */
  if (numero == 4) return 0;
  for (int x = 2; x < numero / 2; x++) {
    // Si es divisible por cualquiera de estos números, no
    // es primo
    if (numero % x == 0) return 0;
  }
  // Si no se pudo dividir por ninguno de los de arriba, sí es primo
  return 1;
}

void ProgramError(long x ){
	if(x>=11|| x<=0){
		printf("Entrada no valida. Asegurese de que el numero de hilos este entre 1 y 10.");
		exit(-1);
	}
	
}

int Tellme_lines(char *Archivo){
	char ca;
	int count=0;

	FILE *file=fopen(Archivo, "rt");    // abrimos el archivo en modo lectura
		if (file==NULL){
			perror("Error en la apertura del archivo.");
		return 1;
		}
	
		while(1){
			 ca = fgetc(file);

      	  if(ca == '\n'){
        	    count=count+1;
        	}
        	if(ca == EOF){  //Si el caracter es end of file imprimimos el contador y salimos del while
          	  break;
        	}

		}
		fclose(file);

		return count;
}
