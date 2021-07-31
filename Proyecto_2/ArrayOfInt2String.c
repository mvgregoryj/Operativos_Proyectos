/** Funcion que transforma un array int a string */
char * ArrayOfInt2String(int *array, int ncol){
	
	char * temp;
	temp = malloc(sizeof(char)*1000);

	for(int i=0; i<ncol; i++){
		sprintf(&temp[i], "%d", array[i]);
	}

	return temp;
	free(temp);
}