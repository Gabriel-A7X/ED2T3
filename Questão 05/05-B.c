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
    Funcionarios **itens; //Onde cada local da tabela tera uma certa estrutura, no caso 'Funcionario'.
};

typedef struct Hash Hash;

Hash *criaHash(int TamanhoHash);
void liberaHash(Hash *hs);
int funcaoHash(char *chave, int TamanhoHash);
Funcionarios inserirFuncionarios();
int insereHash_comColisao(Hash *Ha, Funcionarios funcio, int *count);
int funcaoHash(char *chave, int TamanhoHash);
int funcaoColisao(int pos);
int buscaHash(Hash *Ha, char *mat);
void imprimeFuncionario(Funcionarios *func);
long getMicrotime();

int main(void)
{
    int TamanhoHash = 101;
    srand(time(NULL));
    Hash *Hs;
    Hs = criaHash(TamanhoHash);
    int Menu, loopWhile = 1, n1, Tempo1, Tempo2, tempoGeral = 0, count = 0, x, flag = 0;
    char Matricula[15];
    while (loopWhile)
    {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("1 - Inserir mil funcionarios.\n2 - Buscar Funcionario.\n3 - Sair.\n>>> ");
        scanf("%d", &Menu);
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        switch (Menu)
        {
        case 1:
            for (x = 0; x < 1000; x++)
            {
                Tempo1 = getMicrotime();
                n1 = insereHash_comColisao(Hs, inserirFuncionarios(), &count);
                Tempo2 = getMicrotime();
                tempoGeral += Tempo2 - Tempo1;
            }
            printf("Tempo de Insercao: %d ms\t\t--\t\tTotal de Colisoes: %d\n", tempoGeral, count);
            break;
        case 2:
            printf("Informe a matricula: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]s", Matricula);
            n1 = buscaHash(Hs, Matricula);
            break;
        case 3:
            loopWhile = 0;
            break;
        default:
            printf("Error: Informacao errada!\n");
            break;
        }
    }

    liberaHash(Hs);
    return 0;
}

long getMicrotime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

Funcionarios inserirFuncionarios()
{
    int Valor;
    Funcionarios novo;
    Valor = (rand() % 899999) + 100000;
    sprintf(novo.Matricula, "%d", Valor);
    strcpy(novo.Nome, "TESTE");
    strcpy(novo.Funcao, "FUNCAO TESTE");
    novo.Salario = (rand() % 999999) + 1;
    return novo;
}

void imprimeFuncionario(Funcionarios *func)
{
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
    if (strlen(funcio.Matricula) == 6)
    {
        char chave[10];
        strcpy(chave, funcio.Matricula);
        //Chamando a função para informar a posicao que será guardada a informação na tabela.
        int pos = funcaoHash(chave, Ha->TamanhoHash);
        Funcionarios *novo;
        novo = (Funcionarios *)malloc(sizeof(Funcionarios));
        if (novo == NULL)
        {
            printf("Erro na criacao da estrutura 'Funcionario'\n");
            return 0;
        }
        *novo = funcio;
        //Se nao houver nada na posição informada é so guardar.
        if (Ha->itens[pos] == NULL || Ha->qtd >= Ha->TamanhoHash)
        {
            Ha->itens[pos] = novo;
            Ha->qtd++;
        }
        else
        {
            //Se houver algum dado será procurado uma nova posição, quando achar o loop será interrompido.
            (*count)++;
            int i, newPos = pos, Key = 1;
            for (i = 0; i < Ha->qtd && Key && newPos < Ha->TamanhoHash; i++)
            {
                newPos = funcaoColisao(newPos);
                if (Ha->itens[newPos] == NULL)
                {
                    Ha->itens[newPos] = novo;
                    Ha->qtd++;
                    Key = 0;
                }
                else
                {
                    (*count)++;
                }
            }
            //Se houve o caso de nao encontrar nenhuma posição livre, o dado será guardado na primeira posição que foi encontrado.
            if (Key)
            {
                Ha->itens[pos] = novo;
            }
        }
        return 1;
    }
    else
    {
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
    //Traduções:
    //----------
    //atoi: String para Inteiro.
    //itoa: Inteiro para String.

    int valor1, valor2;
    char Aux1[10], Aux2[10], *ptr;
    //Pegando os valores das posições 0/2/4 da chave e transformando em inteiro.
    Aux1[0] = chave[0];
    Aux1[1] = chave[2];
    Aux1[2] = chave[4];
    Aux1[3] = '\0';
    valor1 = atoi(Aux1);
    //Pegando os valores das posições 1/3/5 da chave e transformando em inteiro.
    Aux1[0] = chave[1];
    Aux1[1] = chave[3];
    Aux1[2] = chave[5];
    Aux1[3] = '\0';
    valor2 = atoi(Aux1);
    valor1 += valor2;
    valor1 %= 1000;         //Pegar apenas os 3 ultimos digitos.
    //Retornando o resto da divisão da soma dos valores 1 e 2 pelo tamanho da tabela.
    return ((int)valor1 % TamanhoHash);
}

int funcaoColisao(int pos)
{
    //Quando der colisão essa função irá somar 7 a posição.
    return pos + 7;
}

int buscaHash(Hash *Ha, char *mat)
{
    if (Ha == NULL)
    {
        return 0;
    }
    char Matricula[10];
    strcpy(Matricula, mat);
    int pos;
    pos = funcaoHash(mat, Ha->TamanhoHash);
    if (Ha->itens[pos] == NULL)
    {
        //Se a posição inicial já for NULL então já pode-se afirmar que a matricula não está inserido ainda
        printf("Error: Nao foi possivel achar esse funcionario!\n");
        return 0;
    }
    else
    {
        //Se houver as condições irão procurar o funcionario na tabela.
        if (strcmp(Ha->itens[pos]->Matricula, Matricula) == 0)
        {
            //Se nao tiver ocorrido nenhuma colisão a matricula inicial estara na primeira posição.
            //Obs. Há uma comparação com a matricula para saber se realmente os dados na posição pertence ao requirido
            imprimeFuncionario(Ha->itens[pos]);
        }
        else
        {
            //Se nao for, então ocorreu uma colisão, então havera uma pesquisa para achar onde está o valor original.
            int newPos = pos, i, Key = 1;
            for (i = 0; i < Ha->TamanhoHash && Key; i++)
            {
                newPos = funcaoColisao(newPos);
                if (Ha->itens[newPos] != NULL && strcmp(Ha->itens[newPos]->Matricula, Matricula) == 0)
                {
                    //Achou o valor da matricula.
                    imprimeFuncionario(Ha->itens[newPos]);
                    Key = 0;
                }
            }
            if (Key == 1)
            {
                //Caso pecorra todas as possiveis posições e nao encontre o valor requirido.
                printf("Error: Nao foi possivel achar esse funcionario!\n");
            }
        }
    }
    return 1;
}