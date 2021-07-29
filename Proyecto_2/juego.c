#include "Lista_Enlazada.c"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int Tellme_lines(char *Archivo);
int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo);
void Imprimir(int ** ma, int nfilas, int ncol);
void juego(int Num_process, int filas, int columnas, char *archivo);
void lector(int Proceso, int Num_process);
void escritor(int Proceso, int Num_process);

int **pipes_primario;
int **pipes_secundario;
long Num_process;
int **Mundito;


int main(int argc, char *argv[] ){

	long Num_process;
	long Num_generaciones;
	long Num_visualizaciones;
	int Numero_lineas;

	Nodo *Lineas_de_mundo=NULL;
	Num_process=strtol(argv[1],NULL,10);
	Num_generaciones=strtol(argv[2],NULL,10);
	Num_visualizaciones=strtol(argv[3],NULL,10);
	Numero_lineas=Tellme_lines(argv[4]);

	FILE *file=fopen(argv[4],"rt");
	if (file==NULL){
		perror("Error en la apertura del archivo");
	}
	
	int filas;
	int columnas;
	for(int i=0;i<2;i++){
		if (i==0){
			fscanf(file, "%d",&filas);
		}
		else{
			fscanf(file, "%d", &columnas);
		}
		
	}
	fclose(file);

	juego(Num_process,filas,columnas,argv[4]);
}


void juego(int Num_process, int filas, int columnas, char *archivo){
	
	//pipes[x][0] lee
	//pipes[x][1] escribe

	int pipes_primario[Num_process-1][2];   // Escribira en el proceso i y leera del proceso i-1
	int pipes_secundario[Num_process-1][2]; // Escribira en el proceso i y leera del proceso i+1
	int pids[Num_process];
	int **Mundito[Num_process];
	int nlineas = (filas/Num_process); //Lineas que le tocaria a cada proceso

	for(int i=0; i<Num_process; i++){
		if(pipe(pipes_primario[i]) == -1){
			printf("Error creando el pipe.");
		}
		if(pipe(pipes_secundario[i]) == -1){
			printf("Error creando el pipe.");
		}
	}

	for (int i=0; i<Num_process; i++){
		pids[i] = fork();
		if(pids[i] == -1){
			printf("Error creando el proceso.");
		}

		if (pids[i] == 0){
			//Proceso hijo
			printf("proceso hijo: %d\n", i);
			Mundito[i] = LeerArchivoProceso(filas,columnas,Num_process,i,nlineas,archivo);
			Imprimir(Mundito[i],nlineas,columnas);
						
			for(int j=0; j<Num_process;j++){

				//Se cierran los pipes que no usara el proceso i

				if(i != j){
					close(pipes_primario[j][0]);
					close(pipes_secundario[j][0]);
				}

				if(i+1 != j){
					close(pipes_secundario[j][1]);
				}

				if(i-1 != j){
					close(pipes_primario[j][1]);
				}
			}

			if(i == 0){
				close(pipes_secundario[0][0]);
			}

			if(i == Num_process-1){
				close(pipes_primario[0][0]);
			}

			return;
		}
		wait(NULL); // Espera que cada hijo termine para que venga el siguiente (Esto para ver si funcionan las otras funciones pero se debe quitar porque no tiene sentido crear uno tras otro)
	
		
	}
	
	// El padre espera a los hijos
	for (int i=0; i<Num_process; i++){
		wait(NULL);
	}


}

/** Para leer pipes */
void lector(int Proceso, int Num_process){

	int i = Proceso;
	
	if(i<Num_process-1){
		if(read(pipes_primario[i][0],Mundito[i],sizeof(int)) == -1){
			printf("Error leyendo el pipe\n");
		}
	}

	if(i>0){
		if(read(pipes_secundario[i][0],Mundito[i],sizeof(int)) == -1){
			printf("Error leyendo el pipe\n");
		}
	}

}

/** Para escribir en los pipes */
void escritor(int Proceso, int Num_process){

	int i = Proceso;

	if(i<Num_process-1){
		if(write(pipes_secundario[i+1][1],Mundito[i+1],sizeof(int)) == -1){
			printf("Error escribiendo en el pipe\n");
		}
	}

	if(i>0){
		if(write(pipes_primario[i-1][1],Mundito[i],sizeof(int)) == -1){
			printf("Error escribiendo en el pipe\n");
		}
	}

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

