#include "Lista_Enlazada.c"
#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <pthread.h>

int Tellme_lines(char *Archivo);
void ImprimirMundo(int **Mundo, int filas, int columnas);
int ** Generar_Mundo(char * Archivo, int filas, int columnas);
int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo);
void Imprimir(int ** ma, int nfilas, int ncol);


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
	#define ANSI_COLOR_RED     "\x1b[31m"
	#define ANSI_COLOR_GREEN   "\x1b[32m"
	#define ANSI_COLOR_RESET   "\x1b[0m"

	printf("%d\n",Numero_lineas);


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
	int **Mundo;


	Mundo=Generar_Mundo(argv[4], filas , columnas);

	Imprimir(Mundo,filas,columnas);

	//int** mini_mundo=LeerArchivoProceso(20, 20,10,9,2,argv[4]);

	//for(int i=0;i<2;i++){
	//	for(int j=0;j<20;j++){
		//	printf("%d\n",mini_mundo[i][j]);
		//}
	//}

}

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

int ** Generar_Mundo(char * Archivo, int filas, int columnas){
	Nodo *Lineas_de_mundo=NULL;
	int numero;
	FILE *files=fopen(Archivo,"rt");
	if (files==NULL){
		perror("Error en la apertura del archivo");
	}
	int i=0;
	int j=0;
	while(fscanf(files,"%d",&numero)==1){
		if(i==0 || i==1){
			i++	;
		}
		else{
			if(i==2){
				InsertarInicio(&Lineas_de_mundo,numero);
				i++;
			}
			else{
				InsertarFinal(&Lineas_de_mundo,numero);
				i++;
			}

		}

		
	}
	fclose(files);
	int **Mundo;
	int xnumero;
	Mundo= (int**)malloc(filas*sizeof(int*));
  	for(int i=0;i<filas;i++){
      Mundo[i] = (int*)malloc(columnas*sizeof(int));
      for(int j=0;j<columnas;j++){
        int xnumero=EliminarInicio(&Lineas_de_mundo);
        Mundo[i][j]=xnumero;
      }
    }
    return Mundo;
}

int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo){
	int i=-2;
	int j=0;
	int Empiezo_desde=Proceso*(filas/numero_de_procesos); //Multiplicamos el proceso por la cantidad de lineas que le tocan para saber desde donde empeiza a guardar las lineas
	int** parte_mundo;
	Nodo *Lista_lineas=NULL;

	FILE *flujo=fopen(Archivo,"rt"); // Abrimos el archivo en modo lectura 
	if (flujo==NULL){
		perror("Error en l apertura de larchivo");
	}
	char linea[100];
	char basura[100];
	int xx=0;
	int ii=0;
	bool lista_vacia=true;
	while(feof(flujo)==0){  // Se va iterando sobre las lineas del archivo hasta llegar a la primera linea del proceso en cuestion, luego cada linea se pasa a entero 
		if(i==Empiezo_desde){   // y se va guardando en orden en la lista enlazada
			while(xx<cantidad_de_lineas){
				fgets(linea,100,flujo);
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


	parte_mundo= (int**)malloc(cantidad_de_lineas*sizeof(int*)); // se el doble apuntador que representa el arreglo con el tamaño de las columnas y las lineas que le toca
	for(int i=0;i<cantidad_de_lineas;i++){                        // se va extrayendo de la lista numero por numero y se guarda en el arreglo, finalmente se obtiene la parte del mundo que le toca al proceso
    	parte_mundo[i] = (int*)malloc(columnas*sizeof(int));
    	for(int j=0;j<columnas;j++){
    		int xnumero=EliminarInicio(&Lista_lineas);
        	parte_mundo[i][j]=xnumero;
    	}
    }
    
    return parte_mundo;




}



void ImprimirMundo(int **Mundo, int filas, int columnas){

	char espacio[50]="  ";
	int z=0;
	int k=0;


	while(z<filas){
		k=0;
		while(k<columnas){
			if(k!=columnas-1){
				printf("%d" "%s",Mundo[z][k], espacio);
				k++;
			}
			if(k==columnas-1){
				printf("%d\n", Mundo[z][k]);
				k++;
			}
		}
		z++;
	}
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