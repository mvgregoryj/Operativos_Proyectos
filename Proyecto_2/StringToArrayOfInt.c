#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(){

	char str[] = "123456789";
	int *ArrayTemp = StringToArrayOfInt(str);
	int sizeString = (int)strlen(str);

	//int sizeArray = (int)(sizeof(ArrayTemp)/sizeof(ArrayTemp[0]));

	printf("String value = %s\n", str);

	for (int i = 0; i < sizeString; ++i){
		printf("ArrayTemp[%d] = %d\n", i, ArrayTemp[i] );
	}

}