#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ncol=9;

// Funcion que transforma String en Array de Enteros
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


char * ArrayOfInt2String(int *array, int ncol){
	
	char * temp;
	temp = malloc(sizeof(char)*1000);

	for(int i=0; i<ncol; i++){
		sprintf(&temp[i], "%d", array[i]);
	}

	return temp;
}


int main(){

	char str[] = "123456789";
	//int *ArrayTemp = StringToArrayOfInt(str);
	int sizeString = (int)strlen(str);

	int ArregloTemporal[] = {9,8,7,6,5,4,3,2,1,0};

	//int sizeArray = (int)(sizeof(ArrayTemp)/sizeof(ArrayTemp[0]));

	//printf("String value = %s\n", str);

	/*
	for (int i = 0; i < sizeString; ++i){
		printf("ArrayTemp[%d] = %d\n", i, ArrayTemp[i] );
	}*/

	//Uso la funcion de array to string
	char *cadena = ArrayOfInt2String(ArregloTemporal,10);
	printf("StringTemp: %s\n", cadena);


}