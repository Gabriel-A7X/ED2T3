#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
    int nivel[81];
};

typedef struct grafos Grafos;

struct torre{
    Grafos *gr;
    int **estado;
};

typedef struct torre Torre;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void conectar(Torre *hanoi);
int move1Disco(Torre *hanoi, int origem, int destino, int *pos);
int podeBotar(Torre *hanoi, int destino, int pos);
int converteEstado(Torre *hanoi, int *estado);
void BellmanFord(Torre *hanoi, int **pesos, int ini);

int main (void){
  int i, j, k, l;
  Torre *hanoi=(Torre*)malloc(sizeof(Torre));
  hanoi->gr=NULL;
  hanoi->gr = criaGrafo(81, 1);
  int contPos = 0, estado[4];
  hanoi->estado=(int**)malloc(sizeof(int*)*81);
  for(i=0;i<81;i++){
      hanoi->estado[i]=(int*)malloc(sizeof(int)*4);
  }
  for(i=0; i<3; i++){
      for(j=0; j<3; j++){
          for(k=0; k<3; k++){
              for(l=0; l<3; l++){
                  hanoi->estado[contPos][0] = i+1;
                  hanoi->estado[contPos][1] = j+1;
                  hanoi->estado[contPos][2] = k+1;
                  hanoi->estado[contPos][3] = l+1;
                  contPos++;
              }
          }
      }
  }
  contPos = 0;
  conectar(hanoi);
  printf("Digite o estado inicial da torre[NUMERO SEPARADOS POR ESPACO]:\n");
  scanf("%d%d%d%d", &estado[0], &estado[1], &estado[2], &estado[3]);
  BellmanFord(hanoi, hanoi->gr->pesos, converteEstado(hanoi, estado));
  return 0;
}

Grafos* criaGrafo(int vertice, int ehPonderado){
  Grafos *gr;
  int i;
  gr = (Grafos*)malloc(sizeof(Grafos));
  if(gr != NULL){
      gr->nVertices = vertice;
      gr->ehPonderado = (ehPonderado != 0)? 1:0;
      gr->grau = (int*) calloc(sizeof(int), vertice);
      gr->arestas = (int**) malloc(vertice * sizeof(int*));
      for(i=0; i<vertice; i++){
          gr->arestas[i] = (int*) calloc(vertice, sizeof(int));
      }
      if(gr->ehPonderado){
          gr->pesos = (int**) malloc(vertice * sizeof(int*));
          for(i=0; i<vertice; i++){
              gr->pesos[i] = (int*) calloc(vertice, sizeof(int));
          }
      }
  }
  return gr;
}

void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo){
  if(*gr != NULL){
    if(origem >= 0 && origem <= (*gr)->nVertices){
      if(destino > 0 && destino <= (*gr)->nVertices){
        (*gr)->arestas[origem][(*gr)->grau[origem]] = destino;
      }
      if((*gr)->ehPonderado){
        (*gr)->pesos[origem][(*gr)->grau[origem]] = peso;
      }
      (*gr)->grau[origem]++;
      if(!ehDigrafo){
        insereAresta(gr, destino, origem, peso, 1);
      }
    }
  }
}

void BellmanFord(Torre *hanoi, int **pesos, int ini){
  Grafos *gr = hanoi->gr;
  int Alterou = 1;
  int n1, n2, n3;
  float vetorCusto[gr->nVertices], vetorAnterior[gr->nVertices];
  for (n1 = 0; n1 < gr->nVertices; n1++){
    vetorCusto[n1] = INFINITY;
  }

  vetorCusto[ini] = 0;
  vetorAnterior[ini] = ini;

  for (n1 = 0; n1 < gr->nVertices - 1 && Alterou; n1++){       //Pecorre todos as ITERAÇÕES possiveis até está tudo correto.
    Alterou = 0;
    for (n2 = 0; n2 < gr->nVertices; n2++){                    //Pecorre todos os VERTICES.
      if (vetorCusto[n2] != INFINITY){
        for (n3 = 0; n3 < gr->grau[n2]; n3++){                  //Pecorre todas as ARESTAS dos VERTICES.
          if (vetorCusto[gr->arestas[n2][n3]] > vetorCusto[n2] + pesos[n2][n3]){
            vetorCusto[gr->arestas[n2][n3]] = vetorCusto[n2] + pesos[n2][n3];
            vetorAnterior[gr->arestas[n2][n3]] = n2;
            Alterou = 1;
          }
        }
      }
    }
  }
  int x = 80, i;
  n2 = x;
  n1 = (int)vetorCusto[x];
  int Aux[n1 + 1];
  for (i = 0; i < n1; i++){
    x = vetorAnterior[x];
    Aux[i] = x;
  }
  printf("Melhor Caminho:\n");
  for (int i = n1 - 1; i >= 0; i--){
    printf(">>> %d%d%d%d\n", hanoi->estado[Aux[i]][0], hanoi->estado[Aux[i]][1], hanoi->estado[Aux[i]][2], hanoi->estado[Aux[i]][3]);
  }
  printf(">>> %d%d%d%d\n", hanoi->estado[80][0], hanoi->estado[80][1], hanoi->estado[80][2], hanoi->estado[80][3]);

}

int converteEstado(Torre *hanoi, int *estado){
    int i, result = -1;
    for(i=0; i<81; i++){
        if(hanoi->estado[i][0] == estado[0] && hanoi->estado[i][1] == estado[1] && hanoi->estado[i][2] == estado[2] && hanoi->estado[i][3] == estado[3]){
            result = i;
        }
    }
    return result;
}

int move1Disco(Torre *hanoi, int origem, int destino, int *pos){
    int i, cont = 0;
    for(i=0; i<4; i++){
        if(hanoi->estado[origem][i] != hanoi->estado[destino][i]){
            *pos = i;
            cont++;
        }
    }
    if(cont != 1){
        cont = 0;
    }
    return cont;
}

int podeBotar(Torre *hanoi, int destino, int pos){
    int i, result = 1;
    for(i=pos+1; i<4; i++){
        if(hanoi->estado[destino][pos] == hanoi->estado[destino][i]){
            result = 0;
        }
    }
    return result;
}

void conectar(Torre *hanoi){
    int i, j, pos;
    for(i=0; i<81; i++){
        for(j=i; j<81; j++){
            if(move1Disco(hanoi, i, j, &pos)){
                if(podeBotar(hanoi, i, pos)){
                    if(podeBotar(hanoi, j, pos)){
                        insereAresta(&hanoi->gr, i, j, 1, 0);
                    }
                }
            }
        }
    }
}