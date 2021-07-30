#include "Lista_Enlazada.c"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//Para colores verde y rojo
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int Tellme_lines(char *Archivo);
int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo);
void Imprimir(int ** ma, int nfilas, int ncol);
int **PrimerTrabajo(int Proceso,int filas, int columnas, int numero_de_procesos, char *Archivo);
Nodo *TraducirMensaje(char *mensaje,int ncol);
void cerrar_pipes(int i);
int **LeerPipe(int i, int Num_process, int ncol);
void EscribePipe(int i, int ** parte_mundo, int Num_process, int num_lineas);


int **pipe_primario, **pipe_secundario, **pipe_terciario;
long Num_process;
int **parte_mundo;
int Cantidad_trabajo, filas, columnas;


int main(int argc, char *argv[] ){

	//Declaracion de variables
	long Num_process;
	long Num_generaciones;
	long Num_visualizaciones;
	int Numero_lineas, filas, columnas;

	Nodo *Lineas_de_mundo=NULL;
	Num_process=strtol(argv[1],NULL,10);
	Num_generaciones=strtol(argv[2],NULL,10);
	Num_visualizaciones=strtol(argv[3],NULL,10);
	Numero_lineas=Tellme_lines(argv[4]);

	FILE *file=fopen(argv[4],"rt");
	if (file==NULL){
		perror("Error en la apertura del archivo");
	}
	
	//Se leen el numero de filas y columnas del archivo
	for(int i=0;i<2;i++){
		if (i==0){
			fscanf(file, "%d",&filas);
		}
		else{
			fscanf(file, "%d", &columnas);
		}
		
	}
	fclose(file);

	//Declaracion de pipes
	pid_t *pids=malloc(sizeof(pid_t) * Num_process);
	int pipe_primario[Num_process-1][2];
	int pipe_secundario[Num_process-1][2];
	int pipe_terciario[Num_process-1][2];
	int ** parte_mundo[Num_process];
	int k=0;

	//pipes[x][0] lee
	//pipes[x][1] escribe

	//Creacion de pipes
	for(int x=0;x<Num_process-1;x++){
		if(pipe(pipe_primario[x]) == -1){
			printf("Error creando el pipe.");
		}
		if(pipe(pipe_secundario[x]) == -1){
			printf("Error creando el pipe.");
		}
		if(pipe(pipe_terciario[x]) == -1){
			printf("Error creando el pipe.");
		}			
	}
	
	//Creacion de procesos	
	for (int i = 0; i < Num_process; ++i){
		pid_t pid = fork();

		if(pid == -1){
			printf("Error creando el proceso.");
		}

		if(pid == 0 && k == 0){
			//Proceso hijo
			printf("proceso hijo: %d\n", i);
			parte_mundo[i] = PrimerTrabajo(i,filas,columnas,Num_process,argv[4]);
			Imprimir(parte_mundo[i],Cantidad_trabajo,columnas);
			//printf("\n");

			return 0;
			
		}
		pids[i]=pid;
		wait(NULL);	// Espera que cada hijo termine para que venga el siguiente 
					// (Esto para ver si funcionan las funcion de imprimir y PrimerTrabajo pero se debe quitar 
					// porque no tiene sentido crear uno tras otro)
	}

	//El padre espera a los hijos
	for (int i = 0; i < Num_process; i++) {
        waitpid(pids[i], NULL, 0);
    }

}


/**Funcion para asignar a cada proceso las lineas que le corresponden*/
int **PrimerTrabajo(int Proceso, int filas, int columnas, int numero_de_procesos, char *Archivo){

	int **parte_mundo;

	if(Proceso != numero_de_procesos-1){
		Cantidad_trabajo = filas/numero_de_procesos; //Cantidad de lineas que le toca a un proceso
		parte_mundo = LeerArchivoProceso(filas,columnas,numero_de_procesos,Proceso,Cantidad_trabajo,Archivo);
		return parte_mundo;
	}

	if(Proceso == numero_de_procesos-1){
		Cantidad_trabajo = filas/numero_de_procesos+(filas%numero_de_procesos); //Cantidad de lineas que le toca al ultimo proceso
		parte_mundo = LeerArchivoProceso(filas,columnas,numero_de_procesos,Proceso,Cantidad_trabajo,Archivo);
		return parte_mundo;
	}
}


/**Funcion para cerrar los pipes que no use el proceso*/
void cerrar_pipes(int i){
	
	for(int j=0; j<Num_process;j++){

		//Se cierran los pipes de lectura que no usara el proceso i
		if(i%2==0 && i==0 && i!=j){									//Cuando es el proceso 0
			close(pipe_secundario[j][0]);
		}

		if(i%2==0 && i==0){											//Cuando es el proceso 0 no se usa el primario
			close(pipe_primario[j][0]);								//entonces se cierran todos 
		}

		if(i%2==0 && i==Num_process-1 && i-1!=j){					//Cuando el proceso es par y es el ultimo
			close(pipe_primario[j][0]);
		}

		if(i%2==0 && i==Num_process-1){							    //Cuando el proceso es par y es el ultimo, como no
			close(pipe_secundario[j][0]);							//usa los secundarios se cierran todos estos
		}

		if(i%2!=0 && i==Num_process-1 && i-1!=j){					//Cuando el proceso es impar y es el ultimo
			close(pipe_secundario[j][0]);					
		}

		if(i%2!=0 && i==Num_process-1){								//Cuando el proceso es impar y es el ultimo, como no
			close(pipe_primario[j][0]);								//usa los primarios se cierran todos estos
		}

		if(i%2==0 && i!=0 && i!=Num_process-1 && i!=j && i-1!=j){	//Cuando el proceso es par y no es el ultimo
			close(pipe_secundario[j][0]);
		}

		if(i%2==0 && i!=0 && i!=Num_process-1){						//Cuando el proceso es par y no es el ultimo, como no
			close(pipe_primario[j][0]);								//usa los primarios se cierran todos estos
		}

		if(i%2!=0 && i!=Num_process-1 && i!=j && i-1!=j){			//Cuando el proceso es impar y no es el ultimo
			close(pipe_primario[j][0]);
		}

		if(i%2!=0 && i!=Num_process-1){								//Cuando el proceso es impar y no es el ultimo, como no
			close(pipe_secundario[j][0]);							//usa los secundarios se cierran todos estos
		}
		
		//TO DO: Se cierran los pipes de escritura que no usara el proceso i 
		
	}


}


/** Para leer pipes */
int **LeerPipe(int i, int Num_process, int ncol){

	//Declaracion de variables
	int **linea_vecina1=(int**)malloc(ncol*sizeof(int));
	int **linea_vecina2=(int**)malloc(ncol*sizeof(int));
	char mensaje1[500];
	char mensaje2[500];

	if(i%2==0 && i==0){
		read(pipe_secundario[i][0],mensaje1,sizeof(mensaje1));
		//Traducir mensaje a int *
		//linea_vecina=mesnaje traducido;
		return linea_vecina1;
	}

	if(i%2==0 && i==Num_process-1){
		read(pipe_primario[i-1][0], mensaje1,sizeof(mensaje1));
		//Traducir mensaje a int *
		//linea_vecina=mesnaje traducido;
		return linea_vecina1;
	}

	if(i%2!=0 && i==Num_process-1){
		read(pipe_secundario[i-1][0], mensaje1,sizeof(mensaje1));
		//Traducir mensaje a int *
		//linea_vecina=mesnaje traducido;
		return linea_vecina1;
	}

	if(i%2==0 && i!=0 && i!=Num_process-1){
		read(pipe_secundario[i-1][0], mensaje1,sizeof(mensaje1));
		read(pipe_secundario[i][0], mensaje2,sizeof(mensaje2));
		//Traducir mensaje1 a int *
		//linea_vecina=mesnaje1 traducido;
		//Traducir mensaje2 a int *
		//linea_vecina=mesnaje traducido;
		
		return linea_vecina1; //  UY se retorna dos hay que retornar una estructura :/ o hacer arreglo de arreglo 
	}

	if(i%2!=0 && i!=Num_process-1){
		read(pipe_primario[i-1][0], mensaje1,sizeof(mensaje1));
		read(pipe_primario[i][0], mensaje2,sizeof(mensaje2));
		//Traducir mensaje1 a int *
		//linea_vecina=mesnaje1 traducido;
		//Traducir mensaje2 a int *
		//linea_vecina=mesnaje traducido;
		
		return linea_vecina1; //  UY se retorna dos hay que retornar una estructura :/ o hacer arreglo de arreglo 
	}

}


/** Para escribir en los pipes */
void EscribePipe(int i, int ** parte_mundo, int Num_process, int num_lineas){ 
	
	char mensaje[500]; 
 	char mensaje2[500]; 

 	if(i%2==0 && i==0){ 
		// pasar de int * parte_mundo[Num_lineas-1] a char [] mensaje 
		write(pipe_primario[0][1], &mensaje ,sizeof(mensaje)); 
	} 

	if(i%2==0 && i==Num_process-1 ){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		write(pipe_primario[i-1][1], &mensaje, sizeof(mensaje)); 
	} 
	
	if(i%2!=0 && i==Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		write(pipe_secundario[i-1][i],&mensaje, sizeof(mensaje)); 
	} 
	
	if(i%2!=0 && i!=Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		write(pipe_secundario[i-1][1],&mensaje,sizeof(mensaje)); 
		// pasar de int * parte_mundo[Num_process-1] a char [] mensaje2 
		write(pipe_secundario[i][1],&mensaje2,sizeof(mensaje2)); 
	} 

	if(i%2==0 && i!=0 && i!=Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		write(pipe_primario[i-1][1],&mensaje,sizeof(mensaje)); 
		// pasar de int * parte_mundo[Num_process-1] a char [] mensaje 
		write(pipe_primario[i][1],&mensaje2,sizeof(mensaje2)); 
	} 
}



Nodo *TraducirMensaje(char *mensaje,int ncol){
	Nodo *lista_numeros=NULL;
	int numero;
	int i=0;
	while(i<ncol){
		numero=mensaje[i]-'0';
		if(i==0){
			InsertarInicio(&lista_numeros,numero);
			i++;
		}
		if (i!=0){
			InsertarFinal(&lista_numeros,numero);
			i++;
		}
	}
	return lista_numeros;

}


/** Funcion para imprimir la matriz */
void Imprimir(int ** ma, int nfilas, int ncol){

  	int m,n;

 	//Se imprime la matriz
  	for(m=0;m<nfilas;m++){		
        printf("\t\t\t\t");
        for(n=0;n<ncol;n++){
      
            //Si es 1 la celula esta viva, por lo que se imprime un 1
            if(ma[m][n] == 1){
                printf(ANSI_COLOR_GREEN   " 1 "   ANSI_COLOR_RESET); // Verde si la celula esta viva
            }
      		//Si no, esta muerta y se imprime un 0
            else{
                printf(ANSI_COLOR_RED     " 0 "     ANSI_COLOR_RESET); // Rojo si la celula esta muerta
            }
        }
        //Salto de linea al llegar al final de la fila
        printf("\n");
    }
	
}


/**Funcion donde cada proceso lee 
del archivo las lineas que le toca
y luego se guardan en un sub-arreglo bidimensional*/
int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo){
	int i=-2;
	int j=0;
	int Empiezo_desde=Proceso*(filas/numero_de_procesos); //Multiplicamos el proceso por la cantidad de lineas que le tocan para saber desde donde empieza a guardar las lineas
	int** parte_mundo;
	Nodo *Lista_lineas=NULL;

	FILE *flujo=fopen(Archivo,"rt"); // Abrimos el archivo en modo lectura 
	if (flujo==NULL){
		perror("Error en la apertura de larchivo");
	}
	char linea[100];
	char basura[100];
	int xx=0;
	int ii=0;
	bool lista_vacia=true;
	
	while(feof(flujo)==0){  // Se va iterando sobre las lineas del archivo hasta llegar a la primera linea del proceso en cuestion, luego cada linea se pasa a entero 
		if(i==Empiezo_desde){   // y se va guardando en orden en la lista enlazada
			while(xx<cantidad_de_lineas){
				fgets(linea,cantidad_de_lineas*columnas,flujo);
				ii=0;
				while(linea[ii]!='\0'){
					if(ii%2==0){
						int numero=linea[ii]-'0';
						if(lista_vacia==true){
							InsertarInicio(&Lista_lineas,numero);
							lista_vacia=false;
						}
						else{
							InsertarFinal(&Lista_lineas,numero);
						}
						ii++;

					}
					else{
						ii++;
					}
				}
				xx++;
			}
			break;
		}	
		else{
			fgets(basura,100,flujo);
			i++;
		}
	}

	fclose(flujo);

	parte_mundo = (int**)malloc(cantidad_de_lineas*sizeof(int*)); // se el doble apuntador que representa el arreglo con el tamaño de las columnas y las lineas que le toca
	for(int i=0;i<cantidad_de_lineas;i++){                        // se va extrayendo de la lista numero por numero y se guarda en el arreglo, finalmente se obtiene la parte del mundo que le toca al proceso
    	parte_mundo[i] = (int*)malloc(columnas*sizeof(int));
    	for(int j=0;j<columnas;j++){
    		int xnumero=EliminarInicio(&Lista_lineas);
        	parte_mundo[i][j]=xnumero;
    	}
    }
    
    return parte_mundo;

}


/** Funcion para saber cuantas lineas tiene el archivo*/
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

