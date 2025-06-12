#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
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
void* cria_exec_mem(unsigned char* codigo, size_t tamanho) {
    int pagesize = sysconf(_SC_PAGESIZE);
    int alloc_size = ((tamanho + pagesize - 1) / pagesize) * pagesize;

    // Criar mmap com permissão de escrita e execução
    void* mem = mmap(NULL, alloc_size, PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(mem == MAP_FAILED)
	{
        perror("mmap");
        exit(1);
    }

    // Copia o código para a memória alocada
    memcpy(mem, codigo, tamanho);
    return mem;
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

	//executa o codigo
	funcp f = (funcp)codigo;
	int i = (int)(*f);

	printf("resultado do codigo:%d\n", i);
	return 0;
}
