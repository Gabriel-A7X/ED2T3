#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int insereHash_comColisao(Hash *Ha, Funcionarios funcio);
int funcaoHash(char *chave, int TamanhoHash);
int funcaoColisao(int pos);
int buscaHash(Hash *Ha, char *mat);
void imprimeFuncionario(Funcionarios *func);

int main(void)
{
    Hash *Hs;
    Hs = criaHash(150);
    int Menu, loopWhile = 1, n1;
    char Matricula[15];
    while (loopWhile)
    {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("1 - Inserir um novo funcionario.\n2 - Buscar Funcionario.\n3 - Sair.\n>>> ");
        scanf("%d", &Menu);
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        switch (Menu)
        {
        case 1:
            if (insereHash_comColisao(Hs, inserirFuncionarios()))
            {
                printf("Funcionario Inserido com Sucesso!\n");
            }
            else
            {
                printf("Error: Funcionario nao inserido na Tabela!\n");
            }
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

Funcionarios inserirFuncionarios()
{
    Funcionarios novo;
    printf("Informe a matricula: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", novo.Matricula);
    printf("Informe o nome: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", novo.Nome);
    printf("Informe a funcao: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", novo.Funcao);
    printf("Informe o salario: ");
    scanf("%f", &novo.Salario);
    return novo;
}

void imprimeFuncionario(Funcionarios *func)
{
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("Matricula:\t\t--\t%s\nNome:\t\t\t--\t%s\nFuncao:\t\t\t--\t%s\nSalario:\t\t--\t%.2f\n",
           func->Matricula, func->Nome, func->Funcao, func->Salario);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

int insereHash_comColisao(Hash *Ha, Funcionarios funcio)
{
    if (Ha == NULL || Ha->qtd == Ha->TamanhoHash)
    {
        printf("Erro na Tabela!\n");
        return 0;
    }
    if (strlen(funcio.Matricula) == 6){
        char chave[10];
        strcpy(chave, funcio.Matricula);
        //Chamando a função para informar a posicao que será guardada a informação na tabela.
        int pos = funcaoHash(chave, Ha->TamanhoHash);
        printf("Posicao: %d\n", pos);
        Funcionarios *novo;
        novo = (Funcionarios *)malloc(sizeof(Funcionarios));
        if (novo == NULL)
        {
            printf("Erro na criacao da estrutura 'Funcionario'\n");
            return 0;
        }
        *novo = funcio;
        //Se nao houver nada na posição informada é so guardar.
        if (Ha->itens[pos] == NULL)
        {
            Ha->itens[pos] = novo;
            Ha->qtd++;
        }
        else
        {
            //Se houver algum dado será procurado uma nova posição, quando achar o loop será interrompido.
            int i, newPos = pos, Key = 1;
            for (i = 0; i < Ha->TamanhoHash && Key; i++)
            {
                newPos = funcaoColisao(newPos);
                if (Ha->itens[newPos] == NULL)
                {
                    Ha->itens[newPos] = novo;
                    Ha->qtd++;
                    Key = 0;
                }
            }
            //Se houve o caso de nao encontrar nenhuma posição livre, o dado será guardado na primeira posição que foi encontrado.
            if (Key == 1)
            {
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