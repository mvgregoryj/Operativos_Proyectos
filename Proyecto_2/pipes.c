#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESS_NUM 3

int main(int argc, char* argv[]){
	int pids[PROCESS_NUM];
	int pipes[PROCESS_NUM +1][2];
	int i;

	for (int i = 0; i < PROCESS_NUM + 1; i++){
		/* code */
		if (pipe(pipes[i]) == -1){
			printf("Error con la creación de pipe\n");		
			return 1;	
		}
	}

	for (int i = 0; i < PROCESS_NUM; ++i){
		pids[i] = fork();

		if (pids[i] == -1){
			printf("Error con la creación de procesos\n");
			return 2;
		}
		if (pids[i]==0){
			//Child process 
			int j;
			for (j = 0; j < PROCESS_NUM + 1; j++){

				// Cerramos las tuberias de lectura que no coincidan con el indice
				if (i != j){
					close(pipes[j][0]);
				}

				// Cerramos las tuberias de escritura que no coincidan con el indice que le corresponde
				if (i + 1!= j){
					close(pipes[j][1]);
				}
			}
			int x;
			if (read(pipes[i][0], &x, sizeof(int)) == -1 ){
				printf("Error leyendo en el pipe\n");
				return 3;
			}
			printf("(%d) Got %d\n", i, x);
			x++;
			if (write(pipes[i+1][1], &x, sizeof(int)) == -1 ){
				printf("Error escribiendo en el pipe\n");
				return 4;
			}

			printf("(%d) Sent %d\n", i, x);

			close(pipes[i][0]);
			close(pipes[i+1][1]);

			return 0;
		}
	}

	// MAIN PROCESS
	int y = 5;

	printf("Main porcess envia %d\n", y);
	if (write(pipes[0][1], &y, sizeof(int)) == -1 ){
		printf("Error escribiendo en el pipe\n");
		return 4;
	}
	if (write(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1 ){
		printf("Error leyendo en el pipe\n");
		return 3;
	}
	printf("El resultado final es %d\n", y);


	// Nos aseguramos que todos los procesos secundarios terminene la ejecución correctamente
	for (int i = 0; i < PROCESS_NUM; i++){
		wait(NULL);
	}
	return 0;
}