
#include "Lista_Enlazada.c"

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>


int Tellme_lines(char *Archivo);
int esPrimo(int numero);
void Separacion_lineas(Nodo **a[],int Num_process,int M,Nodo **Lista_Numeros, Nodo **P1,Nodo **P2,Nodo **P3,Nodo **P4,Nodo **P5,Nodo **P6,Nodo **P7,Nodo **P8,Nodo **P9);
char DeEnteroaCaracter(int numero);
void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_Tareas);
void inicializarProcesos(Nodo **Lista_Tareas, int Proceso,int Tareas);
void AsignarProceso(Nodo **Lista_Tareas, int Proceso,int Tareas);
int main(int argc, char *argv[])
{
	if(strcmp(argv[2],"-p")==0||strcmp(argv[2],"-P")==0){

		char ca;
		int count=Tellme_lines(argv[1]);
		int Numero=0;
		int i=0;
		long Num_process;
		Nodo **a[9];  // arreglo de apuntadores donde iran el apuntador a conjuntos de lineas las culaes se les asignaran a los Procesos hijos por el padre.
		Nodo *Lista_Numeros=NULL;  //Inicializa la lista de Numeros que seran comprobados.
		Nodo *P1,*P2,*P3,*P4,*P5,*P6,*P7,*P8,*P9=NULL;
		
		Nodo *Lista_Tareas=NULL;  // Inicializa Una lista donde estaran en cada Nodo la cantidad de tareas que realizara cada Proceso/Hilo.
		Num_process=strtol(argv[3],NULL,10);  //Numero de procesos solicitados por consola 

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
		Separacion_lineas(a,Num_process,count,&Lista_Numeros,&P1,&P2,&P3,&P4,&P5,&P6,&P7,&P8,&P9);
		int wpid;
		pid_t	pid;
		int fd[2];
		pipe(fd);
		char buf[10];
		int N=Num_process-1;
		int Tareas=count/N;
		inicializarProcesos(&Lista_Tareas,0,Tareas);

		for(i=0;i<N;i++){
			pid=fork();    //Se hace fork()
			if(pid==0){		// si el proceso se crea bien, terminamos el ciclo for
				sleep(1);
				break;

			}else if(pid==-1){
				perror("Herror al hacer fork()");
				exit(1);
				break;
			}else{
				if(i!=0 && i!=N-1){
					AsignarProceso(&Lista_Tareas,i,Tareas);
				}
				else if(i==N-1){
					Tareas=(count/N)+(count%N);
					printf("%d\n",Tareas );
					AsignarProceso(&Lista_Tareas,i,Tareas);
				}
				sleep(2);

			}
		}
		if(pid==0){
			ImprimirLista(Lista_Tareas);
				printf("\n");
			//printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
      exit(0);

		}
		else{
			for(int i=0;i<Num_process;i++){
				if((wpid=wait(NULL))>=0){
					 //printf("Proceso: %d  terminado\n",wpid);
       

				}
			}
		}



		//a[0]=&P1;
		//InsertarInicio(a[0],10000);
		//Nodo *b[9];
		//b[8]=P9;

		//ImprimirLista(b[8]);
		//printf("%ld",count/(Num_process-1)+(count%(Num_process-1)));
		//ImprimirLista(P1);
		//printf("\n\n");
		//ImprimirLista(P2);
		//printf("\n\n");
		//ImprimirLista(P3);
		//printf("\n\n");
		//ImprimirLista(P4);
		//Comprobar_Numero_Primo(a,Num_process,2);
		//har caca;
		//caca=DeEnteroaCaracter(50);
		//printf("%c", caca);


      
	}	

	if(strcmp(argv[2],"-t")==0||strcmp(argv[2],"-T")==0){
		
		char ca;
		int count=Tellme_lines(argv[1]);
		int Numero=0;
		int i=0;
		long Num_process;
		Num_process=strtol(argv[3],NULL,10);  //Numero de procesos solicitados por consola 
		

		FILE *file=fopen(argv[1], "rt");
		if (file==NULL){
			perror("Error en la apertura del archivo");
		return 1;
		}

		int Lines[count];
		while(!feof(file)&&i!=count){
		
			fscanf(file,"%d",&Numero);
			Lines[i]=Numero;
			i=i+1;  //El número de lineas

		}
		fclose(file);


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
	int count;

	FILE *file=fopen(Archivo, "rt");    // abrimos el archivo en modo lectura
		if (file==NULL){
			perror("Error en la apertura del archivo");
		return 1;
		}
	
		while(1){
			 ca = fgetc(file);

      	  if(ca == '\n'){
        	    count++;
        	}
        	if(ca == EOF){  //Si el caracter es end of file imprimimos el contador y salimos del while
          	  break;
        	}

		}
		fclose(file);

		return count;
}

void Separacion_lineas(Nodo **a[],int Num_process,int M,Nodo **Lista_Numeros, Nodo **P1,Nodo **P2,Nodo **P3,Nodo **P4,Nodo **P5,Nodo **P6,Nodo **P7,Nodo **P8,Nodo **P9){
	a[0]=P1,a[1]=P2,a[2]=P3,a[3]=P4,a[4]=P5,a[5]=P6,a[6]=P7,a[7]=P8,a[8]=P9;
	int N=Num_process-1;
	int Tarea1= M/N;
	int Tarea2=M/N+(M%N);
	int x;

	for(int i=0;i<N;i++){
		if(i!=N-1){
			for(int j=0;j<Tarea1;j++){
				x=EliminarInicio(Lista_Numeros);
				InsertarInicio(a[i],x);
			}
		}
		else{
			for(int j=0;j<Tarea2;j++){
				x=EliminarInicio(Lista_Numeros);
				InsertarInicio(a[i],x);
			}
		}
	}

}
char DeEnteroaCaracter(int numero){
 	return numero +'0';
 	
}

void Comprobar_Numero_Primo(Nodo **a[], int Numero_de_proceso, int Numero_Tareas)
{	
	int N=Numero_de_proceso-1;
	char Texto_completo[50];
	char texto2[]=".txt";
	char texto1[50];
	int Chequear_Numero;
	char Numero_chequeado[50];

	for(int i=0 ; i<N;i++){
		sprintf(texto1, "%d", i);
		strcat(texto1,texto2);;
		FILE *file=fopen(texto1,"w");
		if(file==NULL){
			perror("Error en la creacion del archivo \n\n");

		}else{

			for(int j=0;j<Numero_Tareas;j++){
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
}

void inicializarProcesos(Nodo **Lista_Tareas, int Proceso,int Tareas){
	InsertarInicio(Lista_Tareas,Tareas);
	InsertarFinal(Lista_Tareas,Proceso);
}
void AsignarProceso(Nodo **Lista_Tareas, int Proceso,int Tareas){
	EliminarInicio(Lista_Tareas);
	EliminarInicio(Lista_Tareas);
	InsertarInicio(Lista_Tareas,Tareas);
	InsertarFinal(Lista_Tareas,Proceso);

}
