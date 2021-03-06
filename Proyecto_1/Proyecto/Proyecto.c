/**
 * @file Proyecto.c
 * @author Jonathan Bautista 16-10109
 * 		   Daniela Ramirez 16-10940
 * 		   Gregory Mu;oz 16-11313
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
#include <sys/time.h>

/* Declaracion de variables y funciones */

int Tellme_lines(char *Archivo);
int esPrimo(int numero);
void Separacion_lineas(Nodo **a[],int Num_process,int M,Nodo **Lista_Numeros,Nodo **P0, Nodo **P1,Nodo **P2,Nodo **P3,Nodo **P4,Nodo **P5,Nodo **P6,Nodo **P7,Nodo **P8,Nodo **P9, Nodo **Lista_Tareas);
void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_Tareas);
void *Crear_hilos(void *arg);
void *ImprimirNumeroPrimoHilo(void *arg);
void inicializarProcesos(Nodo **Lista_Tareas, int Proceso,int Tareas);
void ProgramError(long x );

struct Parametros{
	int Numero_Hilos;
	int NumeroLineas;
	Nodo *Lista_Numeros;
};

struct Parametros_{
	int Hilo;
	int Tarea;
	Nodo *Lista_N;
};

struct timeval *t0, *t1;


/* Programa principal que se encarga de leer el archivo .txt 
*  y crear procesos o hilos dependiendo de lo que se pase por 
*  la terminal
*/

int main(int argc, char *argv[]){
	
	t0 = malloc(sizeof(struct timeval)); // Se inicializa la variable t0 para calcular el tiempo
	t1 = malloc(sizeof(struct timeval)); // Se inicializa la variable t1 para calcular el tiempo

	/* Si el usuario quiere crear procesos */
	if(strcmp(argv[2],"-p")==0||strcmp(argv[2],"-P")==0){

		gettimeofday(t0, NULL); // Se obtiene el tiempo inicial

		char ca;
		int count=Tellme_lines(argv[1]);
		int Numero=0;
		int i=0;
		long Num_process;

		Nodo **a[10];  // arreglo de apuntadores donde iran el apuntador a conjuntos de lineas las culaes se les asignaran a los Procesos hijos por el padre.
		Nodo *Lista_Numeros=NULL;  //Inicializa la lista de Numeros que seran comprobados.
		Nodo *P0=NULL;
		Nodo *P1=NULL;
		Nodo *P2=NULL;
		Nodo *P3=NULL;
		Nodo *P4=NULL;
		Nodo *P5=NULL;
		Nodo *P6=NULL;
		Nodo *P7=NULL;
		Nodo *P8=NULL;
		Nodo *P9=NULL;	
		Nodo *Lista_Tareas=NULL;  // Inicializa una lista donde estaran en cada Nodo la cantidad de tareas que realizara cada Proceso/Hilo.
		Num_process=strtol(argv[3],NULL,10);  //Numero de procesos solicitados por consola 
		ProgramError(Num_process);
		FILE *file=fopen(argv[1], "rt");
		if (file==NULL){
			perror("Error en la apertura del archivo");
		return 1;
		}

		while(!feof(file)&&i!=count){
			if(i==0){
				fscanf(file,"%d",&Numero); // extrae del archivo el numero de la linea.
				InsertarInicio(&Lista_Numeros, Numero); // inserta en la lista enlazada el numero respectivo de cada linea
				i=i+1;  //El n??mero de lineas
			}
			fscanf(file,"%d",&Numero);
			InsertarFinal(&Lista_Numeros, Numero);
			i=i+1;  //El n??mero de lineas

		}
		fclose(file);
		Separacion_lineas(a,Num_process,count,&Lista_Numeros,&P0,&P1,&P2,&P3,&P4,&P5,&P6,&P7,&P8,&P9,&Lista_Tareas);

		
		int wpid;
		pid_t	pid;
		int N=Num_process;		
		
		for(i=0;i<N;i++){

			pid=fork();    //Se hace fork()
			if(pid==0){	   // si el proceso se crea bien, terminamos el ciclo for
				break;

			}else if(pid==-1){
				perror("Error al hacer fork()");
				exit(1);
				break;

			}else{
				EliminarInicio(&Lista_Tareas);
				EliminarInicio(&Lista_Tareas);
				sleep(1);
			}
		}
		if(pid==0){			
			int x=EliminarInicio(&Lista_Tareas);
			int y=EliminarInicio(&Lista_Tareas);
			Comprobar_Numero_Primo(a,x,y);
			
			//printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
      	exit(0);

		}
		else{
			for(int i=0;i<N;i++){
				if((wpid=wait(NULL))>=0){
				}
			}
			gettimeofday(t1, NULL); // Se obtiene el tiempo final
			unsigned int ut1 = t1->tv_sec*1000000+t1->tv_usec;
			unsigned int ut0 = t0->tv_sec*1000000+t0->tv_usec;
			printf("El tiempo de corrida es: %d microsegundos.\n", (ut1-ut0)); /* Tiempo de ejecucion en microsegundos */

			printf("Programa terminado.\n");
		}
      
	}	

	/* Si el usuario quiere crear hilos*/
	if(strcmp(argv[2],"-t")==0||strcmp(argv[2],"-T")==0){

		gettimeofday(t0, NULL);	// Se obtiene el tiempo inicial

		char ca;
		int count=Tellme_lines(argv[1]);
		int Numero=0;
		int i=0;
		long Num_threads;
		Nodo *Lista_Numeros=NULL;
		Nodo *Lista_Tareas=NULL;
		Num_threads=strtol(argv[3],NULL,10);  //Numero de hilos solicitados por consola 
		ProgramError(Num_threads);
		FILE *file=fopen(argv[1], "rt");
		if (file==NULL){
			perror("Error en la apertura del archivo.");
		return 1;
		}

		while(!feof(file)&&i!=count){
			if(i==0){
				fscanf(file,"%d",&Numero); // extrae del archivo el numero de la linea.
				InsertarInicio(&Lista_Numeros, Numero); // inserta en la lista enlazada el numero respectivo de cada linea
				i=i+1;  //El n??mero de lineas
			}
			fscanf(file,"%d",&Numero);
			InsertarFinal(&Lista_Numeros, Numero);
			i=i+1;  //El n??mero de lineas

		}

		pthread_t Master;
		struct Parametros p;
		p.Numero_Hilos=Num_threads;
		p.Lista_Numeros=Lista_Numeros;
		p.NumeroLineas=count;
				
		pthread_create(&Master,NULL, Crear_hilos,(void *)&p); // Creacion del hilo maestro
		pthread_join(Master,NULL); // El hilo maestro espera a que terminen los hilos trabajadores
		
	}

}


/* Funcion para crear los hilos hijos */

void *Crear_hilos(void *arg){
	struct Parametros *p;
	p=(struct Parametros *)arg;
	struct Parametros_ p1;		
	int Tarea1=(p->NumeroLineas)/(p->Numero_Hilos); // Cantidad de lineas que se le asignara a los hilos trabajadores 
	int Tarea2=(p->NumeroLineas)/(p->Numero_Hilos)+(p->NumeroLineas)%(p->Numero_Hilos); // Cantidad de lineas que se le asignara al maestro
	p1.Lista_N=p->Lista_Numeros;
	pthread_t Worker;
			
	for(int i=0;i<p->Numero_Hilos;i++){
			
		if(i!=(p->Numero_Hilos-1)){
		p1.Hilo=i;
		p1.Tarea=Tarea1;
		pthread_create(&Worker,NULL,ImprimirNumeroPrimoHilo,(void *)&p1);
		pthread_join(Worker,NULL);
		}
		else if(i==(p->Numero_Hilos-1)){
		p1.Hilo=i;
		p1.Tarea=Tarea2;
		pthread_create(&Worker,NULL,ImprimirNumeroPrimoHilo,(void *)&p1);
		pthread_join(Worker,NULL);
		}		
	}

	gettimeofday(t1, NULL); // Se obtiene el tiempo final
	unsigned int ut1 = t1->tv_sec*1000000+t1->tv_usec;
	unsigned int ut0 = t0->tv_sec*1000000+t0->tv_usec;	
	printf("El tiempo de corrida es: %d microsegundos.\n", (ut1 - ut0)); /* Tiempo de ejecucion en microsegundos */	
	printf("Programa terminado.\n");

}


/* Funcion que se encarga de imprimir los numeros del archivo de entrada 
*  en un nuevo archivo i.txt donde i es el numero del trabajador, 
*  tal que 0 =< i < 10. En el nuevo archivo, si al lado del numero aparece un 1 
*  el numero es primo, si aparece un 0 no es primo. Para Hilos
*/

void *ImprimirNumeroPrimoHilo(void *arg){
	struct Parametros_ *s;
	s=(struct Parametros_ *)arg;
	char Texto_completo[50];
	char texto2[]=".txt";
	char texto1[50];
	int Chequear_Numero;
	char Numero_chequeado[50];
	int i;
	i=s->Hilo;
	sprintf(texto1, "%d", i);
	strcat(texto1,texto2);
	FILE *file=fopen(texto1,"w");
	
	if(file==NULL){
		perror("Error en la creacion del archivo \n\n");

		}else{

		for(int j=0;j<s->Tarea;j++){
			Chequear_Numero= EliminarInicio(&s->Lista_N);
			if(esPrimo(Chequear_Numero)==1){
				fprintf(file, "%d %c", Chequear_Numero,'1');
			}else{
				fprintf(file, "%d %c",Chequear_Numero,'0');
			}
			fputc('\n',file) ;
			}
			fflush(file);
			fclose(file);
		}

}


/* Funcion que separa en un arreglo de listas la lista de numeros que le toca a cada proceso. 
*  El indice de cada casilla del arreglo es el proceso.
*/

void Separacion_lineas(Nodo **a[10],int Num_process,int M,Nodo **Lista_Numeros,Nodo **P0, Nodo **P1,Nodo **P2,Nodo **P3,Nodo **P4,Nodo **P5,Nodo **P6,Nodo **P7,Nodo **P8,Nodo **P9,Nodo **Lista_Tareas){
	a[0]=P0;
	a[1]=P1;
	a[2]=P2;
	a[3]=P3;
	a[4]=P4;
	a[5]=P5;
	a[6]=P6;
	a[7]=P7;
	a[8]=P8;
	a[9]=P9;
	int N=Num_process;
	int Tarea1= M/N;
	int Tarea2=M/N+(M%N);
	int x;
	if(N!=1){
		for(int i=0;i<N;i++){
			if(i!=(N-1)){
				if (i==0){
					InsertarInicio(Lista_Tareas,i);
					InsertarFinal(Lista_Tareas,Tarea1);
				}else{
					InsertarFinal(Lista_Tareas,i);
					InsertarFinal(Lista_Tareas,Tarea1);
				}
							
				for(int j=0;j<Tarea1;j++){
					x=EliminarInicio(Lista_Numeros);
					InsertarInicio(a[i],x);
				}
			}
			else if(i==(N-1)){
				InsertarFinal(Lista_Tareas,i);
				InsertarFinal(Lista_Tareas,Tarea2);
				for(int j=0;j<Tarea2;j++){
					x=EliminarInicio(Lista_Numeros);
					InsertarInicio(a[i],x);
				}
			}
		}
	}else{
		InsertarInicio(Lista_Tareas,0);
		InsertarFinal(Lista_Tareas,Tarea1);
		for(int j=0;j<Tarea1;j++){
			x=EliminarInicio(Lista_Numeros);
			InsertarInicio(a[0],x);
		}

	}

}


/* Funcion que se encarga de imprimir los numeros del archivo de entrada 
*  en un nuevo archivo i.txt donde i es el numero del trabajador, 
*  tal que 0 =< i < 10. En el nuevo archivo, si al lado del numero aparece un 1 
*  el numero es primo, si aparece un 0 no es primo. Para procesos.
*/

void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_de_Tareas){	
	char Texto_completo[50];
	char texto2[]=".txt";
	char texto1[50];
	int Chequear_Numero;
	char Numero_chequeado[50];
	int i;
	i=Numero_de_proceso;
	sprintf(texto1, "%d", i);
	strcat(texto1,texto2);;
	FILE *file=fopen(texto1,"w");
	if(file==NULL){
		perror("Error en la creacion del archivo \n\n");

		}else{

		for(int j=0;j<Numero_de_Tareas;j++){
			Chequear_Numero= EliminarInicio(a[i]);
			if(esPrimo(Chequear_Numero)==1){
				fprintf(file, "%d %c", Chequear_Numero,'1');
			}else{
				fprintf(file, "%d %c",Chequear_Numero,'0');
			}
			fputc('\n',file) ;
			}
			fflush(file);
			fclose(file);
		}

}


/* Funcion que se encarga de contar las lineas del archivo de entrada. */

int Tellme_lines(char *Archivo){
	char ca;
	int count=0;

	FILE *file=fopen(Archivo, "rt");    // abrimos el archivo en modo lectura
		if (file==NULL){
			perror("Error en la apertura del archivo");
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


/* Funcion que comprueba si un numero es primo o no */

int esPrimo(int numero) {
  if (numero == 0 || numero == 1){

  	return 0;
  }
  /*
          El n??mero 4 es un caso especial, pues al dividirlo entre
          2 el resultado es 2, y el ciclo nunca se cumple, indicando que
          el 4 S?? es primo, pero realmente NO lo es, as?? que si el n??mero es 4
                        inmediatamente indicamos que no es primo (regresando 0)
          Nota: solo es para el 4, los dem??s funcionan bien
  */
  if (numero == 4) return 0;
  for (int x = 2; x < numero / 2; x++) {
    // Si es divisible por cualquiera de estos n??meros, no
    // es primo
    if (numero % x == 0) return 0;
  }
  // Si no se pudo dividir por ninguno de los de arriba, s?? es primo
  return 1;
}


/* Funcion para que el usuario cumpla con la condicion de que ingrese
*  la cantidad correcta de procesos o hilos a crear en caso de que solicite
*  mas de 10 hilos/procesos o menos de 1 
*/

void ProgramError(long x){
	if(x>=11|| x<=0){
		printf("Entrada no valida. Asegurese de que el numero de procesos o hilos este entre 1 y 10.");
		exit(-1);
	}
	
}

