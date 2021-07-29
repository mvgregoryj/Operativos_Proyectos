/* Funcion del Jugo de la vida*/
int ** gameOfLife(int filasM, int columnasN, int Matriz){

    //Declaro las variables que necesitamos
    int **nextGen = (int **)malloc(filasM * sizeof(int *));
    for (j = 0; j < columnasN; j++)
    	nextGen[j] = (int **)malloc(columnasN * sizeof(int *));

    //Instrucciones para retornar la siguiente generación

    // Recorrido de filas
    for (int i = 0; i < filasM; i++){

    	// Recorrido de columnas
    	for (int j = 0; j < columnasN; j++){

    		int vecinosVivos = 0;

    		// Casos de casillas:

    		// Caso 1: Borde superior
    		if (i == 0){

    			// Caso 1.1: Esquina superior izquierda
    			if(j == 0){
	    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
					vecinosVivos = Matriz[i][j+1] + Matriz[i+1][j] + Matriz[i+1][j+1];
    			}

    			// Caso 1.2: Esquina superior derecha
    			else if(j == columnasN - 1){
	    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
					vecinosVivos = Matriz[i][j-1] + Matriz[i+1][j-1] + Matriz[i+1][j];
    			}

    			// Caso 1.3: Borde superior
    			else{
		    		// Verifica estado de los 5 vecinos
		    		for (int x = 0; x <= 1; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x = 0 && y = 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}
    		}

    		// Caso 2: Borde izquierdo sin ser esquina
    		else if((i != 0 || i != filasM - 1) && j == 0){
	    		// Verifica estado de los 5 vecinos
	    		for (int x = -1; x <= 1; x++){

		    		for (int y = 0; y <= 1; y++){

		    			// Se omite contarse a si mismo como vecino
		    			if (x = 0 && y = 0){
		    				continue;
		    			}

		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
	    				vecinosVivos += Matriz[i + x][j + y];

		    		}
	    		}
    		}

    		// Caso 3: Borde derecho sin ser esquina
    		else if((i != 0 || i != filasM - 1) && j == columnasN - 1){
	    		// Verifica estado de los 5 vecinos
	    		for (int x = -1; x <= 1; x++){

		    		for (int y = -1; y <= 0; y++){

		    			// Se omite contarse a si mismo como vecino
		    			if (x = 0 && y = 0){
		    				continue;
		    			}

		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
	    				vecinosVivos += Matriz[i + x][j + y];

		    		}
	    		}
    		}

    		// Caso 4: Borde inferior
    		else if (i == filasM - 1){

    			// Caso 4.1: Esquina inferior izquierda
    			if(j == 0){
	    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
					vecinosVivos = Matriz[i-1][j] + Matriz[i-1][j+1] + Matriz[i][j+1];
    			}

    			// Caso 4.2: Esquina inferior derecha
    			else if(j == columnasN - 1){
	    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
					vecinosVivos = Matriz[i-1][j-1] + Matriz[i-1][j] + Matriz[i][j-1];
    			}

    			// Caso 4.3: Borde inferior
    			else{
		    		// Verifica estado de los 5 vecinos
		    		for (int x = -1; x <= 0; x++){

			    		for (int y = -1; y <= 1; y++){

			    			// Se omite contarse a si mismo como vecino
			    			if (x = 0 && y = 0){
			    				continue;
			    			}

			    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
		    				vecinosVivos += Matriz[i + x][j + y];

			    		}
		    		}
	    		}
    		}

    		// Caso 5: Casilla no esta en ningun extremo o borde
    		else{
	    		// Verifica estado de los 8 vecinos
	    		for (int x = -1; x <= 1; x++){

		    		for (int y = -1; y <= 1; y++){

		    			// Se omite contarse a si mismo como vecino
		    			if (x = 0 && y = 0){
		    				continue;
		    			}

		    			// Se suman los vecinos, el valor total serán los vecinosVivos. 
	    				vecinosVivos += Matriz[i + x][j + y];

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
    return nextGen
}