#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "peqcomp.h"

//Cria arquivo temporario contendo o codigo SBas e termina o programa caso falhe ao criar o arquivo
FILE* cria_temp(unsigned char* codigo)
{
	FILE* tmp = tmpfile();

	if(tmp == NULL)
	{
		printf("Falha ao criar arquivo temporario\n");
		exit(1);
	}
	
	for(int i = 0; i < sizeof(codigo); ++i)
	{
		fputc(codigo[i], tmp);
	}

	rewind(tmp);
	return tmp;
}

//MMAP para que o codigo de maquina seja executavel
unsigned char* mem_sbas(unsigned char* codigo)
{
		//Assumindo um tamanho fixo para cada arquivo temporario pra nao ter que calcular cada arquivo individualmente
		//unsigned char* codigo = (unsigned char*)mmap(NULL);
		return 0;
}

int main()
{
	unsigned char codigo[] = {0x55, 0x48, 0x89, 0xe5, 0x31, 0xc0, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xc9, 0xc3};

	FILE* tmp = cria_temp(codigo);
	int byte;

	for(int i = 0; i < sizeof(codigo); ++i)
	{
		byte = fgetc(tmp);	
		printf("codigo %d: %02x\n", i, byte);
	}
	printf("\n");
	printf("tamanho do codigo:%d\n", sizeof(codigo));
	return 0;
}
