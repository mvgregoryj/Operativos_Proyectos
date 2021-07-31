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

int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo);
void Imprimir(int ** ma, int nfilas, int ncol);
int **PrimerTrabajo(int Proceso,int filas, int columnas, int numero_de_procesos, char *Archivo);
Nodo *TraducirMensaje(char *mensaje,int ncol);
void cerrar_pipes(int i);
void EscribePipe(int i, int ** parte_mundo, int Num_process, int num_lineas);
int ** gameOfLife(int filasM, int columnasN, int ** Matriz, int *partesMundo1, int *partesMundo2, int procesosTotales, int proceso);
// void generaciones(int filasM, int columnasN, int ** Matriz, int n_generaciones, int n_visualizaciones);
int * StringToArrayOfInt(char* str);
char * ArrayOfInt2String(int *array, int ncol);
int Cantidad_de_trabajo(int Proceso, int filas, int numero_de_procesos);
void ImprimirArreglo(int * array, int ncol);



int **pipe_primario, **pipe_secundario, **pipe_terciario;
long Num_process;
int **parte_mundo;
int columnas;




struct Partes_de_mundo{
	int * partes_mundo1;
	int * partes_mundo2;
};

typedef struct Partes_de_mundo Partes_de_mundo; 

Partes_de_mundo LeerPipe(int i, int Num_process, int ncol);


int main(int argc, char *argv[] ){

	//Declaracion de variables
	int Num_process;
	int Num_generaciones;
	int Num_visualizaciones;
	int filas;

	int ***GeneracionesTotales[Num_process]; 							// Arreglo donde se almacenaran todas las generaciones de todos los procesos

	Nodo *Lineas_de_mundo=NULL;
	Num_process=strtol(argv[1],NULL,10);
	Num_generaciones=strtol(argv[2],NULL,10);
	Num_visualizaciones=strtol(argv[3],NULL,10);

	int cantidadParaVizualizar = Num_generaciones/Num_visualizaciones + Num_generaciones%Num_visualizaciones;

	int GenAVisualizar[cantidadParaVizualizar];


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
	int pipe_terciario[Num_process][2];
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
	}
	for(int x=0;x<Num_process;x++){
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
			int ** parte_mundo[Num_process];


			// Se cierran los pipes innecesarios
			cerrar_pipes(i);

			//

			int *partesMundo1, *partesMundo2;

			printf("\nproceso hijo: %d\n", i);
			parte_mundo[i] = PrimerTrabajo(i,filas,columnas,Num_process,argv[4]);

			Imprimir(parte_mundo[i], Cantidad_de_trabajo(i, filas, Num_process), columnas);
			printf("\n");

			//Se escribe el pipe
			printf("Fila 0 del parte_mundo[%d] = \n", i);
			ImprimirArreglo(parte_mundo[i][0], columnas);
			//Imprimir(parte_mundo, Cantidad_de_trabajo(i, filas, Num_process), columnas);
			EscribePipe(i, parte_mundo[i], Num_process, Cantidad_de_trabajo(i, filas, Num_process));

			printf("Hola2\n");

			//Se lee el pipe
			Partes_de_mundo pM;
			pM = LeerPipe(i, Num_process, columnas);

			printf("Hola3\n");

			/////////////////// GENERACIONES ///////////////////

				// Variables para generaaciones:
				int Generaciones[Num_generaciones]; 							// Arreglo donde se almacenaran todas las generaciones


				// Iteraciones para almacenar las generaciones el el arreglo
				for (int l = 0; l < Num_generaciones; ++l){

					//Si es la generación 0:
					if (l==0){
						Generaciones[l] = parte_mundo[i];
					}
					// Si no es la generación 0
					else{
						
						Generaciones[l] = gameOfLife(Cantidad_de_trabajo(i, filas, Num_process), columnas, Generaciones[l-1], pM.partes_mundo1, pM.partes_mundo2, Num_process, i);
								
						//Se escribe el pipe
						EscribePipe(i, Generaciones[l], Num_process, Cantidad_de_trabajo(i, filas, Num_process));

						//Se lee el pipe
						pM = LeerPipe(i, Num_process, columnas);

					}
				}

				GeneracionesTotales[i] = Generaciones;


		/////////////////// /////////////////// ///////////////////
			


			Imprimir(parte_mundo[i],Cantidad_de_trabajo(i, filas, Num_process),columnas);
			printf("\n");



			return 0;
			
		}
		pids[i]=pid;
		//wait(NULL);	// Espera que cada hijo termine para que venga el siguiente 
					// (Esto para ver si funcionan las funcion de imprimir y PrimerTrabajo pero se debe quitar 
					// porque no tiene sentido crear uno tras otro)
	}


	//El padre espera a los hijos
	for (int i = 0; i < Num_process; i++) {
        waitpid(pids[i], NULL, 0);

        /////////////////////////////////////////////////////////


        for (int i = 0; i < Num_process; ++i){

			// Iteraciones para guardar en un arreglo solo las generaciones cada n_visualizaciones
			for (int r = 0; r < cantidadParaVizualizar; r++){

				GenAVisualizar[r] = GeneracionesTotales[i][r*Num_visualizaciones];
				
				printf("Generación %d: \n", i*Num_visualizaciones);

				ImprimirArregloDeMatrices(GeneracionesTotales[i*Num_visualizaciones], Cantidad_de_trabajo(i, filas, Num_process), columnas);
			}

        }



    }

}

/*Funcion para calcular la cantidad de trabajo*/
int Cantidad_de_trabajo(int Proceso, int filas, int numero_de_procesos){

	int Cantidad_trabajo;

	if(Proceso != numero_de_procesos-1){
		Cantidad_trabajo = filas/numero_de_procesos; //Cantidad de lineas que le toca a un proceso
		//printf("cantidad de trabajo proceso %d = %d\n", Proceso, Cantidad_trabajo);
	}

	if(Proceso == numero_de_procesos-1){
		Cantidad_trabajo = filas/numero_de_procesos+(filas%numero_de_procesos); //Cantidad de lineas que le toca al ultimo proceso
		//printf("cantidad de trabajo proceso %d = %d\n", Proceso, Cantidad_trabajo);
	}
	return Cantidad_trabajo;
}



/**Funcion para asignar a cada proceso las lineas que le corresponden*/
int **PrimerTrabajo(int Proceso, int filas, int columnas, int numero_de_procesos, char *Archivo){

	int **parte_mundo;
	int Cantidad_trabajo;

	Cantidad_trabajo = Cantidad_de_trabajo(Proceso, filas, numero_de_procesos); //Cantidad de lineas que le toca a un proceso
	parte_mundo = LeerArchivoProceso(filas,columnas,numero_de_procesos,Proceso,Cantidad_trabajo,Archivo);

	printf("Cantidad de trabajo proceso %d = %d\n", Proceso, Cantidad_trabajo);

	return parte_mundo;

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
		

		//Se cierran los pipes de escritura que no usara el proceso i 
		if(i%2==0 && i==0 && i!=j){									//Cuando es el proceso 0
			close(pipe_primario[j][1]);
		}

		if(i%2==0 && i==0){											//Cuando es el proceso 0 no se usa el secundario
			close(pipe_secundario[j][1]);							//entonces se cierran todos 
		}

		if(i%2==0 && i==Num_process-1 && i-1!=j){					//Cuando el proceso es par y es el ultimo
			close(pipe_primario[j][1]);
		}

		if(i%2==0 && i==Num_process-1){							    //Cuando el proceso es par y es el ultimo, como no
			close(pipe_secundario[j][1]);							//usa los secundarios se cierran todos estos
		}

		if(i%2!=0 && i==Num_process-1 && i-1!=j){					//Cuando el proceso es impar y es el ultimo
			close(pipe_secundario[j][1]);					
		}

		if(i%2!=0 && i==Num_process-1){								//Cuando el proceso es impar y es el ultimo, como no
			close(pipe_primario[j][1]);								//usa los primarios se cierran todos estos
		}

		if(i%2==0 && i!=0 && i!=Num_process-1 && i!=j && i-1!=j){	//Cuando el proceso es par y no es el ultimo
			close(pipe_primario[j][1]);
		}

		if(i%2==0 && i!=0 && i!=Num_process-1){						//Cuando el proceso es par y no es el ultimo, como no
			close(pipe_secundario[j][1]);							//usa los secundarios se cierran todos estos
		}

		if(i%2!=0 && i!=Num_process-1 && i!=j && i-1!=j){			//Cuando el proceso es impar y no es el ultimo
			close(pipe_secundario[j][1]);
		}

		if(i%2!=0 && i!=Num_process-1){								//Cuando el proceso es impar y no es el ultimo, como no
			close(pipe_primario[j][1]);	  							//usa los primarios se cierran todos estos
		}
		
	}

}


/**Funcion para leer el pipe*/
Partes_de_mundo LeerPipe(int i, int Num_process, int ncol){

	//Declaracion de variables
	Partes_de_mundo p;
	char mensaje1[500];
	char mensaje2[500];

	if(i%2==0 && i==0){
		if(read(pipe_secundario[i][0],&mensaje1,sizeof(mensaje1)) == -1){
			printf("Error leyendo en el pipe\n");
			//Traducir mensaje a int *
			// p.partes_mundo2=Traducir(mensaje1);
			p.partes_mundo2 = StringToArrayOfInt(mensaje1);
			return p;
		}
	}

	if(i%2==0 && i==Num_process-1){
		if(read(pipe_secundario[i-1][0], &mensaje1,sizeof(mensaje1)) == -1){
			printf("Error leyendo en el pipe\n");
			//Traducir mensaje a int *
			// p.partes_mundo1=Traducir(mensaje1);
			p.partes_mundo1 = StringToArrayOfInt(mensaje1);
			return p;
		}
			
	}

	if(i%2!=0 && i==Num_process-1){
		if(read(pipe_primario[i-1][0], &mensaje1,sizeof(mensaje1)) == -1){
			printf("Error leyendo en el pipe\n");
			//Traducir mensaje a int *
			// p.partes_mundo1=Traducir(mensaje1);
			p.partes_mundo1 = StringToArrayOfInt(mensaje1);
			return p;

		}
		
	}

	if(i%2==0 && i!=0 && i!=Num_process-1){

		read(pipe_secundario[i-1][0], &mensaje1,sizeof(mensaje1));
		read(pipe_secundario[i][0], &mensaje2,sizeof(mensaje2));

		//Traducir mensaje1 a int *
		// p.partes_mundo1=Traducir(mensaje1);
		p.partes_mundo1 = StringToArrayOfInt(mensaje1);

		//Traducir mensaje2 a int *
		// p.partes_mundo2=Traducir(mensaje2);
		p.partes_mundo2 = StringToArrayOfInt(mensaje2);
	
		
		return p; 
	}

	if(i%2!=0 && i!=Num_process-1){

		read(pipe_primario[i-1][0], mensaje1,sizeof(mensaje1));
		read(pipe_primario[i][0], mensaje2,sizeof(mensaje2));

		//Traducir mensaje1 a int *
		// p.partes_mundo1=Traducir(mensaje1);
		p.partes_mundo1 = StringToArrayOfInt(mensaje1);

		//Traducir mensaje2 a int *
		// p.partes_mundo2=Traducir(mensaje2);
		p.partes_mundo2 = StringToArrayOfInt(mensaje2);
		
		return p; 
	}

}


/** Para escribir en los pipes */
void EscribePipe(int i, int ** parte_mundo, int Num_process, int num_lineas){ 

	printf("holaPipe1\n");
	
	//Declaracion de variables
	//char *mensaje; 
 	//char *mensaje2; 

 	if(i%2==0 && i==0){
		ImprimirArreglo(parte_mundo[num_lineas - 1],columnas);
		// pasar de int * parte_mundo[Num_lineas-1] a char [] mensaje 

		//printf("mensaje: %s", ArrayOfInt2String(parte_mundo[num_lineas-1], columnas));

		char *mensaje = ArrayOfInt2String(parte_mundo[num_lineas-1], columnas);
    	//mensaje = ArrayOfInt2String(parte_mundo[num_lineas-1], columnas);

		printf("mensaje: %s", mensaje);

		write(pipe_primario[0][1], &mensaje ,sizeof(mensaje)); 
	} 

	

	if(i%2==0 && i==Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		char *mensaje = ArrayOfInt2String(parte_mundo[0], columnas);
		write(pipe_primario[i-1][1], &mensaje, sizeof(mensaje)); 
	} 
	
	if(i%2!=0 && i==Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		char *mensaje = ArrayOfInt2String(parte_mundo[0], columnas);
		write(pipe_secundario[i-1][1],&mensaje, sizeof(mensaje)); 
	} 
	
	if(i%2!=0 && i!=Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje
		char *mensaje = ArrayOfInt2String(parte_mundo[0], columnas); 
		write(pipe_secundario[i-1][1],&mensaje,sizeof(mensaje));

		// pasar de int * parte_mundo[num_lineas-1] a char [] mensaje2
		char *mensaje2 = ArrayOfInt2String(parte_mundo[num_lineas-1], columnas); 
		write(pipe_secundario[i][1],&mensaje2,sizeof(mensaje2)); 
	} 

	if(i%2==0 && i!=0 && i!=Num_process-1){ 
		// pasar de int * parte_mundo[0] a char [] mensaje 
		char *mensaje = ArrayOfInt2String(parte_mundo[0], columnas);
		write(pipe_primario[i-1][1],&mensaje,sizeof(mensaje));

		// pasar de int * parte_mundo[num_lineas-1] a char [] mensaje2 
		char *mensaje2 = ArrayOfInt2String(parte_mundo[num_lineas-1], columnas);
		write(pipe_primario[i][1],&mensaje2,sizeof(mensaje2)); 
	} 
	printf("Escribepipe_final\n");

}


/** Funcion que transforma String en Array de Enteros */
int * StringToArrayOfInt(char* str){

	int *ArrayTemp;
	int sizeString = (int)strlen(str);

	//int sizeArray = (int)(sizeof(ArrayTemp)/sizeof(ArrayTemp));
	//printf("sizeArray = %d\n", sizeArray);

    ArrayTemp = (int *)malloc(sizeString*sizeof(int*));

	printf("strlen(%s)=%d\n",str,(int)strlen(str));


	for (int i = 0; i < sizeString; ++i){
		ArrayTemp[i] = str[i]-'0';
	}

	return ArrayTemp;
}


/** Funcion que transforma un array int a string */
char * ArrayOfInt2String(int *array, int ncol){
	
	char * temp;
	temp = malloc(sizeof(char)*1000);

	for(int i=0; i<ncol; i++){
		sprintf(&temp[i], "%d", array[i]);
	}

	printf("StringTemp: %s\n", temp);

	return temp;
	//free(temp);
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

/** Funcion para imprimir la arreglo de matrices */
void ImprimirArregloDeMatrices(int *** ma, int Num_generaciones, int nfilas, int ncol){

  	int i,m,n;

 	//Se imprime la matriz
 	for (i = 0; i < Num_generaciones; ++i){
 	 	for(m=0;m<nfilas;m++){		
	        printf("\t\t\t\t");
	        for(n=0;n<ncol;n++){
	      
	            //Si es 1 la celula esta viva, por lo que se imprime un 1
	            if(ma[i][m][n] == 1){
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

/** Funcion para imprimir la arreglos */
void ImprimirArreglo(int * array, int ncol){

  	int n;

 	//Se imprime la arreglos
    for(n=0;n<ncol;n++){
  
        //Si es 1 la celula esta viva, por lo que se imprime un 1
        if(array[n] == 1){
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


/**Funcion donde cada proceso lee 
del archivo las lineas que le toca
y luego se guardan en un sub-arreglo bidimensional*/
int ** LeerArchivoProceso(int filas, int columnas, int numero_de_procesos, int Proceso, int cantidad_de_lineas, char *Archivo){
	int i=-2;
	int j=0;
	int Empiezo_desde = Proceso*(filas/numero_de_procesos); //Multiplicamos el proceso por la cantidad de lineas que le tocan para saber desde donde empieza a guardar las lineas
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
	
	while(feof(flujo)==0){      // Se va iterando sobre las lineas del archivo hasta llegar a la primera linea del proceso en cuestion, luego cada linea se pasa a entero 
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

/* Funcion del Jugo de la vida*/
int ** gameOfLife(int filasM, int columnasN, int ** Matriz, int *partesMundo1, int *partesMundo2, int procesosTotales, int proceso){

    //Declaro las variables que necesitamos
    int** nextGen;

    nextGen = (int **)malloc(filasM*sizeof(int*));
    for (int i = 0; i < filasM; i++){
    	nextGen[i] = (int*)malloc(columnasN*sizeof(int));
    }

    //Instrucciones para retornar la siguiente generación

    // Recorrido de filas
    for (int i = 0; i < filasM; i++){

    	// Recorrido de columnas
    	for (int j = 0; j < columnasN; j++){

    		//	printf("i=%d , j=%d\n",i,j);

    		int vecinosVivos = 0;

    		// Casos por cantidad de procesos

    		// Caso 1: Nos encontramos en el 1er proceso y existen más de 1 procesosTotales
    		if (proceso == 1 && proceso < procesosTotales){

	    		// Casos de casillas:

	    		// Caso 1.1: Borde superior
	    		if (i == 0){

	    			// Caso 1.1.1: Esquina superior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
	    			}

	    			// Caso 1.1.2: Esquina superior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
	    			}

	    			// Caso 1.1.3: Borde superior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = 0; x <= 1; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
		    		}
	    		}

	    		// Caso 1.2: Borde inferior
	    		else if (i == filasM - 1){

	    			//printf("Borde Inferior\n");


	    			// Caso 1.2.1: Esquina inferior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1] + partesMundo2[j] + partesMundo2[j+1];
	    			}

	    			// Caso 1.2.2: Esquina inferior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1] + partesMundo2[j-1] + partesMundo2[j];
	    			}

	    			// Caso 1.2.3: Borde inferior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = -1; x <= 0; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}

			    		vecinosVivos = vecinosVivos + partesMundo2[j-1] + partesMundo2[j] + partesMundo2[j+1];
		    		}
	    		}

	    		// Caso 1.3: Borde izquierdo sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == 0){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = 0; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 1.4: Borde derecho sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 0; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 1.5: Casilla no esta en ningun extremo o borde
	    		else{
		    		// Verifica estado de los 8 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
		    	}
    		}

    		// Caso 2: Nos encontramos en un proceso medio
    		else if (1 < proceso < procesosTotales){

	    		// Casos de casillas:

	    		// Caso 2.1: Borde superior
	    		if (i == 0){

	    			// Caso 2.1.1: Esquina superior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = partesMundo1[j] + partesMundo1[j+1] + Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
	    			}

	    			// Caso 2.1.2: Esquina superior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = partesMundo1[j-1] + partesMundo1[j] + Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
	    			}

	    			// Caso 2.1.3: Borde superior sin ser esquina sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = 0; x <= 1; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
			    		vecinosVivos = vecinosVivos + partesMundo1[j-1] + partesMundo1[j] + partesMundo1[j+1];
		    		}
	    		}

	    		// Caso 2.2: Borde inferior
	    		else if (i == filasM - 1){

	    			//printf("Borde Inferior\n");


	    			// Caso 2.2.1: Esquina inferior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1] + partesMundo2[j] + partesMundo2[j+1];
	    			}

	    			// Caso 2.2.2: Esquina inferior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1] + partesMundo2[j-1] + partesMundo2[j];
	    			}

	    			// Caso 2.2.3: Borde inferior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = -1; x <= 0; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}

			    		vecinosVivos = vecinosVivos + partesMundo2[j-1] + partesMundo2[j] + partesMundo2[j+1];
		    		}
	    		}

	    		// Caso 2.3: Borde izquierdo sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == 0){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = 0; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 2.4: Borde derecho sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 0; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 2.5: Casilla no esta en ningun extremo o borde
	    		else{
		    		// Verifica estado de los 8 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
		    	}
    		}

    		// Caso 3: Nos encontramos en el ultimo proceso, equivalente al numero de procesosTotales
    		else if (1 < proceso && proceso == procesosTotales){

	    		// Caso 3.1: Borde superior
	    		if (i == 0){

	    			// Caso 3.1.1: Esquina superior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = partesMundo1[j] + partesMundo1[j+1] + Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
	    			}

	    			// Caso 3.1.2: Esquina superior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = partesMundo1[j-1] + partesMundo1[j] + Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
	    			}

	    			// Caso 3.1.3: Borde superior sin ser esquina sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = 0; x <= 1; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
			    		vecinosVivos = vecinosVivos + partesMundo1[j-1] + partesMundo1[j] + partesMundo1[j+1];
		    		}
	    		}

	    		// Caso 3.2: Borde inferior
	    		else if (i == filasM - 1){

	    			//printf("Borde Inferior\n");


	    			// Caso 3.2.1: Esquina inferior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1];
	    			}

	    			// Caso 3.2.2: Esquina inferior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1];
	    			}

	    			// Caso 3.2.3: Borde inferior
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = -1; x <= 0; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
		    		}
	    		}

	    		// Caso 3.3: Borde izquierdo sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == 0){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = 0; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 3.4: Borde derecho sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 0; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 3.5: Casilla no esta en ningun extremo o borde
	    		else{
		    		// Verifica estado de los 8 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
		    	}
		    }

    		// Caso 4: Hay un solo proceso Total y nos encontramos en el
    		else if (proceso == 1 && procesosTotales == 1){

	    		// Casos de casillas:

	    		// Caso 4.1: Borde superior
	    		if (i == 0){

	    			// Caso 4.1.1: Esquina superior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
	    			}

	    			// Caso 4.1.2: Esquina superior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
	    			}

	    			// Caso 4.1.3: Borde superior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = 0; x <= 1; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
		    		}
	    		}

	    		// Caso 4.2: Borde inferior
	    		else if (i == filasM - 1){

	    			//printf("Borde Inferior\n");


	    			// Caso 4.2.1: Esquina inferior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1];
	    			}

	    			// Caso 4.2.2: Esquina inferior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1];
	    			}

	    			// Caso 4.2.3: Borde inferior
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = -1; x <= 0; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
		    		}
	    		}

	    		// Caso 4.3: Borde izquierdo sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == 0){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = 0; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 4.4: Borde derecho sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 0; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 4.5: Casilla no esta en ningun extremo o borde
	    		else{
		    		// Verifica estado de los 8 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
		    	}
	    	}

		/*
    		// Caso 5: Hay N procesos y N procesosTotales
    		if (proceso == 1 && proceso = procesosTotales){

	    		// Casos de casillas:

	    		// Caso 5.1: Borde superior
	    		if (i == 0){

	    			// Caso 5.1.1: Esquina superior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
	    			}

	    			// Caso 5.1.2: Esquina superior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
	    			}

	    			// Caso 5.1.3: Borde superior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = 0; x <= 1; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}
		    		}
	    		}

	    		// Caso 5.2: Borde inferior
	    		else if (i == filasM - 1){

	    			//printf("Borde Inferior\n");


	    			// Caso 5.2.1: Esquina inferior izquierda
	    			if(j == 0){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1] + partesMundo2[j] + partesMundo2[j+1];
	    			}

	    			// Caso 5.2.2: Esquina inferior derecha
	    			else if(j == columnasN - 1){
		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
						vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1] + partesMundo2[j-1] + partesMundo2[j];
	    			}

	    			// Caso 5.2.3: Borde inferior sin ser esquina
	    			else{
			    		// Verifica estado de los 5 vecinos
			    		for (int x = -1; x <= 0; x++){

				    		for (int y = -1; y <= 1; y++){

				    			// Se omite contarse a si mismo como vecino
				    			if (x == 0 && y == 0){
				    				continue;
				    			}

				    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
			    				vecinosVivos += Matriz[i + x][j + y];

				    		}
			    		}

			    		vecinosVivos = vecinosVivos + partesMundo2[j-1] + partesMundo2[j] + partesMundo2[j+1];
		    		}
	    		}

	    		// Caso 5.3: Borde izquierdo sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == 0){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = 0; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 5.4: Borde derecho sin ser esquina
	    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 0; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}

	    		// Caso 5.5: Casilla no esta en ningun extremo o borde
	    		else{
		    		// Verifica estado de los 8 vecinos
		    		for (int x = -1; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x == 0 && y == 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
		    	}
    		}
		*/
    		// Reglas del juego

    		// 1. Nacimiento: Célula muerta con exactamente 3 vecinos vivos se convierte en célula viva
    		if (Matriz[i][j] == 0 && vecinosVivos == 3){
    			nextGen[i][j] = 1;
    		}


    		// 2. Supervivencia: Célula viva con 2 o 3 vecinos vivos permanece viva.
    		else if (Matriz[i][j] == 1 && (vecinosVivos == 2 || vecinosVivos == 3)){
    			nextGen[i][j] = 1;
    		}

    		// 3. Muerte: Una célula muere o permance muerta por sobrepoblación o soledad.
    		else{
    			nextGen[i][j] = 0;
    		}
    	}    	
    }
    // Retornamos la matriz nueva
    return nextGen;
}