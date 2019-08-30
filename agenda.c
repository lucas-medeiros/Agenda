#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <unistd.h>

#define MAXTAB  30
#define MAXNOME 64
#define MAXTEL  32

typedef char Nome[MAXNOME];     /* Nome = char [MAXNOME] */

typedef struct {
    int dia;
    int mes;
} DataAniversario;              /* Data do aniversário */

typedef char Telefone[MAXTEL];  /* Telefone = char [MAXTEL] */

typedef struct {
    Nome nome;                 /* nome da pessoa */
    DataAniversario niver;     /* dia e mes do aniversario da pessoa */
    Telefone tel;              /* telefone da pessoa */
} Registro;

typedef Registro Tabela[MAXTAB];  /* Tabela = vetor de MAXTAB Registros */


/* PROGRAMA PRINCIPAL */
int main(int argc, char* argv[])
{
    Tabela tab;
    unsigned int N;        /* no. de entradas na tabela */
    Nome nome_aux;         /* auxiliar temporario para o nome */
    unsigned int i;        /* contador */
    int fd;				   
    char resp;
    N = 0;
    
    if(argc < 2){
		printf("Erro na entrada de parâmetros, insira o nome do arquivo\n");
		return -1;
	}
    
    fd = open(argv[1],O_RDWR); //tenta abrir o arquivo apenas para leitura e escrita
    if (fd == -1){ //caso o arquivo não exista ele é criado
		fd = open(argv[1],O_CREAT|O_RDWR,0666);
		if (fd == -1){ 
			printf("Erro ao abrir o arquivo \n");
			return -1;
		}
		write(fd, &N, sizeof(int)); //garante que a primeira coisa armazenada no arquivo seja o N
	}	
	
	read(fd, &N, sizeof(int)); //primeiros 4 bytes (sizeof(int)) guarda o N
	if (N != 0)
		read(fd, &tab, sizeof(tab)); //le a tabela salva no arquivo
    
    printf("\n*** AGENDA *** 	SO - PUCPR\n\n");
    printf("Sua agenda atualmente possui %d contatos\n", N);  
    printf("Deseja inserir novos contatos? [s/n]\n");
    scanf("%c", &resp);
    
    if((resp == 's') || (resp == 'S')){
		/* Le tabela de nomes/aniversarios/telefones */
		printf("   (Digite \"fim\" no nome para terminar.)\n\n");
		printf("      Nome: ");
		scanf("%s",nome_aux);
		while ((strcmp(nome_aux,"fim") != 0) && (N<MAXTAB)) {  /* enq nome != "fim" */
			strcpy(tab[N].nome,nome_aux);      /* tab[N].nome = nome_aux */
			printf("      Dia aniv.: ");
			scanf("%d",&(tab[N].niver.dia));
			printf("      Mes aniv.: ");
			scanf("%d",&(tab[N].niver.mes));
			printf("      Telefone: ");
			scanf("%s",tab[N].tel);
			N++;
			/* Le proximo nome */
			printf("\n      Nome: ");
			scanf("%s",nome_aux);
		}
    }
    
    /* Grava a tabela no arquivo */
    lseek(fd, 0, SEEK_SET); 		//volta o "ponteiro" para o início do arquivo
    write(fd, &N, sizeof(int));  //escreve o novo valor de N no início do arquivo
    write(fd, &tab, sizeof(tab));	// escreve a nova tabela
    
    /* Procura nome e imprime data de aniversario e tel correspondentes */
    printf("\n>> Lendo nomes da tabela\n");
    printf("   (Digite \"fim\" para terminar.)\n\n");
    printf("      Nome: ");
    scanf("%s",nome_aux);
    
    while (strcmp(nome_aux,"fim") != 0) {
		/* procura nome na tabela */
		for (i=0; i<N; i++) {
			if (strcmp(nome_aux,tab[i].nome) == 0)   /* nome_aux == tab[i].nome */
			break;    /* encontrou */
		}
		if (i == N) {
			/* nome nao enconrtado */
			printf("\n\t* Nome \"%s\" nao encontrado na tabela!\n\n",nome_aux);
		} else {
			/* nome encontrado */
		  printf("\n\t* %s => %d/%d (Tel: %s)\n\n",tab[i].nome,tab[i].niver.dia,tab[i].niver.mes,tab[i].tel);
		}
		/* Le proximo nome */
		printf("      Nome: ");
		scanf("%s",nome_aux);
    }
    
    printf("\nFIM\n");
    close(fd);
    return 0;
}
