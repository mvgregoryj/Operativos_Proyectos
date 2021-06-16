/**
 * @file Proyecto.c
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

#include <signal.h>

int Tellme_lines(char *Archivo);
int esPrimo(int numero);
void Separacion_lineas(Nodo **a[],int Num_process,int M,Nodo **Lista_Numeros,Nodo **P0, Nodo **P1,Nodo **P2,Nodo **P3,Nodo **P4,Nodo **P5,Nodo **P6,Nodo **P7,Nodo **P8,Nodo **P9, Nodo **Lista_Tareas);
void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_Tareas);
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

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	printf("\nI’m sorry Dave. I’m afraid I can’t do that.\n");
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sigintHandler);

	if(strcmp(argv[2],"-p")==0||strcmp(argv[2],"-P")==0){

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
		Nodo *Lista_Tareas=NULL;  // Inicializa Una lista donde estaran en cada Nodo la cantidad de tareas que realizara cada Proceso/Hilo.
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
				i=i+1;  //El número de lineas
			}
			fscanf(file,"%d",&Numero);
			InsertarFinal(&Lista_Numeros, Numero);
			i=i+1;  //El número de lineas

		}
		fclose(file);
		Separacion_lineas(a,Num_process,count,&Lista_Numeros,&P0,&P1,&P2,&P3,&P4,&P5,&P6,&P7,&P8,&P9,&Lista_Tareas);
		int wpid;
		pid_t	pid;
		int N=Num_process;

		for(i=0;i<N;i++){
			pid=fork();    //Se hace fork()
			if(pid==0){		// si el proceso se crea bien, terminamos el ciclo for
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
			printf("Programa Termiando\n");
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

int Tellme_lines(char *Archivo)
{
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


void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_de_Tareas)
{	
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

void ProgramError(long x ){
	if(x>=11|| x<=0){
		printf("Entrada No valida, Asegurece de que el numero de procesos Este entre 1 y 10");
		exit(-1);
	}
	
}
