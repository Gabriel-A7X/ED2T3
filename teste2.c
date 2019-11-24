#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

struct Funcionarios
{
    char Matricula[7], Nome[100], Funcao[100];
    float Salario;
};

typedef struct Funcionarios Funcionarios;

void lerdoArq(char **mats);
long getMicrotime();
int insereHash_comColisao(int *Ha, char *Matricula, int TamanhoHash, int *count, int *QtdInserido);
int funcaoHash(char *chave, int TamanhoHash);
int funcaoColisao(char *chave, int pos);

int main () {
    int TamanhoHash = 150, i, x, tempoGeral = 0, Tempo1, Tempo2, posicao, colisoes = 0, qtdInseridos = 0;
    int TabelaHash[TamanhoHash];
    for (i = 0; i < TamanhoHash; i++){
        TabelaHash[i] = 0;
    }
    srand(time(NULL));
    char **mats;
    mats=(char**)malloc(sizeof(char*)*1000);
    for(i = 0; i < 1000; i++){
        mats[i]=(char*)malloc(sizeof(char)*40);
    }
    lerdoArq(mats);
    for (x = 0; x < 1000; x++){
        Tempo1 = getMicrotime();
        posicao = insereHash_comColisao(TabelaHash, mats[x], TamanhoHash, &colisoes, &qtdInseridos);
        TabelaHash[posicao] = atoi(mats[x]);
        // printf("Valor Original: %s\t\t--\t\tValor depois do ATOI: %d\n", mats[x], atoi(mats[x]));
        // printf("Valor Tabela: %d\n", TabelaHash[posicao]);
        Tempo2 = getMicrotime();
        tempoGeral += Tempo2 - Tempo1;
    }
    printf("Tempo gasto: %d    --    Colisoes: %d\n", tempoGeral, colisoes);

   return(0);
}

void lerdoArq(char **mats){
    FILE *arq = fopen("entradaHASHING.txt", "r");
	if (arq == NULL){
    	printf("ERRO! O arquivo não foi aberto (FUNÇÃO setEntrada)!\n");
	}else{
		int i=0;
		while( (fscanf(arq, "%s", mats[i])) != EOF){
			//printf("%ld = [%s]\n",strlen(func[i].matricula), func[i].matricula );
			i++;
		}
	}
}

long getMicrotime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int insereHash_comColisao(int *Ha, char *Matricula, int TamanhoHash, int *count, int *QtdInserido){
    int pos = funcaoHash(Matricula, TamanhoHash);
    //Se nao houver nada na posição informada é so guardar.
    if (Ha[pos] == 0){
        (*QtdInserido)++;
        return pos;
    }else if ((*QtdInserido) >= TamanhoHash){
        (*count)++;
        (*QtdInserido)++;
        return pos;
    }else{
        (*count)++;
        int i, newPos = pos, Key = 1;
        //Se houver algum dado será procurado uma nova posição, quando achar o loop será interrompido.
        for (i = 0; i < TamanhoHash; i++){
            newPos = funcaoColisao(Matricula, newPos);
            if (Ha[newPos] == 0){
                (*QtdInserido)++;
                return newPos;
            }
            else
            {
                (*count)++;
            }
        }
    }
    return pos;
}

int funcaoHash(char *chave, int TamanhoHash)
{
    int valor = 0;
    char Aux[7];
    //Rotação do dois digitos da direita para a esquerda.
    Aux[0] = chave[4];
    Aux[1] = chave[5];
    Aux[2] = chave[0];
    Aux[3] = chave[1];
    Aux[4] = chave[2];
    Aux[5] = chave[3];
    Aux[6] = '\0';
    strcpy(chave, Aux);
    //Pegando os valores das posições 2/4/6 da nova chave.
    Aux[0] = Aux[1];
    Aux[1] = Aux[3];
    Aux[2] = Aux[5];
    Aux[3] = '\0';
    //Transformando em inteiro e dividindo pelo tamanho da tabela e retornando o resto.
    valor = atoi(Aux);
    return ((int) valor % TamanhoHash);
}

int funcaoColisao(char *chave, int pos)
{
    //Quando der colisão essa função irá somar o resto da divisão com o primeiro dígito da matrícula.
    int Aux = (int)chave[0] - 48;
    return pos + Aux;
}
