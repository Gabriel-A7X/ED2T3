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

struct Hash
{
    int qtd, TamanhoHash;
    Funcionarios **itens;       //Onde cada local da tabela tera uma certa estrutura, no caso 'Funcionario'.
};

typedef struct Hash Hash;

Hash *criaHash(int TamanhoHash);
void liberaHash(Hash *hs);
int funcaoHash(char *chave, int TamanhoHash);
Funcionarios inserirFuncionarios(char *mat);
int insereHash_comColisao(Hash *Ha, Funcionarios funcio, int *count);
int funcaoHash(char *chave, int TamanhoHash);
int funcaoColisao(char *chave, int pos);
long getMicrotime();
void lerdoArq(char **mats);

int main(void)
{
    int TamanhoHash = 101;
    srand(time(NULL));
    Hash *Hs;
    Hs = criaHash(TamanhoHash);
    int Menu, loopWhile = 1, n1, Tempo1, Tempo2, tempoGeral = 0, count = 0, x, flag = 0;
    char **mats;
    mats=(char**)malloc(sizeof(char*)*1000);
    for(int i=0;i<1000;i++){
        mats[i]=(char*)malloc(sizeof(char)*40);
    }

    lerdoArq(mats);
    for (x = 0; x < 1000; x++){
        Tempo1 = getMicrotime();
        n1 = insereHash_comColisao(Hs, inserirFuncionarios(mats[x]), &count);
        Tempo2 = getMicrotime();
        tempoGeral += Tempo2 - Tempo1;
    }
    printf("Tempo de Insercao: %d ms\t\t--\t\tTotal de Colisoes: %d\n", tempoGeral, count);

    liberaHash(Hs);
    return 0;
}

long getMicrotime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
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

Funcionarios inserirFuncionarios(char *mat)
{
    // int Valor;
    Funcionarios novo;
    // Valor = (rand() % 899999) + 100000;
    strcpy(novo.Matricula, mat);
    strcpy(novo.Nome, "TESTE");
    strcpy(novo.Funcao, "FUNCAO TESTE");
    novo.Salario = (rand() % 999999) + 1;
    return novo;
}

void imprimeFuncionario(Funcionarios *func){
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("Matricula:\t\t--\t%s\nNome:\t\t\t--\t%s\nFuncao:\t\t\t--\t%s\nSalario:\t\t--\t%.2f\n",
        func->Matricula, func->Nome, func->Funcao, func->Salario);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

int insereHash_comColisao(Hash *Ha, Funcionarios funcio, int *count)
{
    if (Ha == NULL)
    {
        return 0;
    }
    if (strlen(funcio.Matricula) == 6){
        char chave[10];
        strcpy(chave, funcio.Matricula);
        //Chamando a função para informar a posicao que será guardada a informação na tabela.
        int pos = funcaoHash(chave, Ha->TamanhoHash);
        Funcionarios *novo;
        novo = (Funcionarios *)malloc(sizeof(Funcionarios));
        if (novo == NULL){
            printf("Erro na criacao da estrutura 'Funcionario'\n");
            return 0;
        }
        *novo = funcio;
        //Se nao houver nada na posição informada é so guardar.
        if (Ha->itens[pos] == NULL){
            Ha->itens[pos] = novo;
            Ha->qtd++;
        }else if (Ha->qtd >= Ha->TamanhoHash){
            Ha->itens[pos] = novo;
            (*count)++;     //Conta a colisão pois já ira conter algum valor nessa posição.
        }else{
        //Se houver algum dado será procurado uma nova posição, quando achar o loop será interrompido.
            (*count)++;
            int i, newPos = pos, Key = 1;
            strcpy(chave, funcio.Matricula);
            for (i = 0; i < Ha->TamanhoHash && Key; i++){
                newPos = funcaoColisao(chave, newPos);
                //Se o valor da colisao passar do tamanho do vetor, então não tem logica continuar o loop.
                if (newPos > Ha->TamanhoHash){
                    i = newPos;
                }else if (Ha->itens[newPos] == NULL){
                    Ha->itens[newPos] = novo;
                    Ha->qtd++;
                    Key = 0;
                }else{
                    (*count)++;
                }
            }
            //Se houve o caso de nao encontrar nenhuma posição livre, o dado será guardado na primeira posição que foi encontrado.
            if (Key){
                Ha->qtd++;
                Ha->itens[pos] = novo;
            }
        }
        return 1;
    }else{
        printf("Error: Na digitacao da MATRICULA!!\n");
        return 0;
    }
}

Hash *criaHash(int TamanhoHash)
{
    Hash *hs = (Hash *)malloc(sizeof(Hash));
    if (hs != NULL)
    {
        int i;
        hs->TamanhoHash = TamanhoHash;
        hs->itens = (Funcionarios **)malloc(TamanhoHash * sizeof(Funcionarios *));
        if (hs->itens == NULL)
        {
            free(hs);
            return NULL;
        }
        hs->qtd = 0;
        for (i = 0; i < hs->TamanhoHash; i++)
        {
            hs->itens[i] = NULL;
        }
    }
    return hs;
}

void liberaHash(Hash *hs)
{
    if (hs != NULL)
    {
        int i;
        for (i = 0; i < hs->TamanhoHash; i++)
        {
            if (hs->itens[i] != NULL)
            {
                free(hs->itens[i]);
            }
        }
        free(hs->itens);
        free(hs);
    }
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
    Aux[0] = chave[1];
    Aux[1] = chave[3];
    Aux[2] = chave[5];
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
