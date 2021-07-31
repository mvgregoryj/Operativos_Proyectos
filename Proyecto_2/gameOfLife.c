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
    		else if (1 < proceso && proceso = procesosTotales){

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
    		else if (proceso == 1 && proceso = procesosTotales){

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


/* Funcion para imprimir la generaciones */
void generaciones(int filasM, int columnasN, int ** Matriz, int n_generaciones, int n_visualizaciones){

	int **Generaciones[n_generaciones]; 	// Arreglo donde se almacenaran las generaciones
	int cantidadParaImprimir = n_generaciones/n_visualizaciones + n_generaciones%n_visualizaciones;

	// Iteraciones para almacenar las generaciones el el arreglo
	for (int i = 0; i < n_generaciones; ++i){

		//Si es la 1ra generación
		if (i==0){
			Generaciones[i] = Matriz;
		}
		// Si no es la 1ra generación
		else{
			Generaciones[i] = gameOfLife(filasM, columnasN, Generaciones[i-1]);
		}
	}

	// Iteraciones para imprimir las generaciones cada n_visualizaciones
	for (int i = 0; i < cantidadParaImprimir; i++){
		
		printf("Generación %d: \n", i*n_visualizaciones);

		Imprimir(Generaciones[i*n_visualizaciones], filasM, columnasN);
	}
}
