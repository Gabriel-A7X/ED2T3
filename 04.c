#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct grafos{
    float **pesos;
    int nVertices, ehPonderado, *grau, **arestas;
};

typedef struct grafos Grafos;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, float peso, int ehDigrafo);
void BellmanFord(Grafos *gr, float **pesos, int ini);
void adj(Grafos *gr);

int main(){
    Grafos *gr;
    gr = criaGrafo(8, 1);
    insereAresta(&gr, 0, 1, 0.0, 1);
    insereAresta(&gr, 0, 2, 0.3, 1);
    insereAresta(&gr, 0, 3, 0.8, 1);
    insereAresta(&gr, 1, 3, 0.9, 1);
    insereAresta(&gr, 2, 3, 0.5, 1);
    insereAresta(&gr, 3, 5, 1.0, 1);
    insereAresta(&gr, 3, 4, 1.0, 1);
    insereAresta(&gr, 5, 1, 1.0, 1);
    insereAresta(&gr, 5, 6, 0.2, 1);
    insereAresta(&gr, 4, 5, 0.5, 1);
    insereAresta(&gr, 4, 2, 0.12, 1);
    insereAresta(&gr, 4, 7, 0.4, 1);
    insereAresta(&gr, 6, 4, 0.7, 1);
    insereAresta(&gr, 7, 6, 0.6, 1);
    //adj(gr);
    BellmanFord(gr, gr->pesos, 0);
    return 0;
}

void adj(Grafos *gr){
    for(int i=0;i<8;i++){
        printf("Estado %d \n",i);
        for(int j=0;j<gr->grau[i];j++){
            printf("Vai para o Estado %d com peso %.2f\n",gr->arestas[i][j], gr->pesos[i][j]);
        }
    }
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
          gr->pesos = (float**) malloc(vertice * sizeof(float*));
          for(i=0; i<vertice; i++){
              gr->pesos[i] = (float*) calloc(vertice, sizeof(float));
          }
      }
  }
  return gr;
}

void insereAresta(Grafos **gr, int origem, int destino, float peso, int ehDigrafo){
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

void BellmanFord(Grafos *gr, float **pesos, int ini){
  int n1, n2, n3;
  float vetorCusto[gr->nVertices], vetorAnterior[gr->nVertices];
  for (n1 = 0; n1 < gr->nVertices; n1++){
    vetorCusto[n1] = -INFINITY;
  }

  vetorCusto[ini] = 0;
  vetorAnterior[ini] = ini;

  for (n1 = 0; n1 < gr->nVertices - 1; n1++){       //Pecorre todos as ITERAÇÕES possiveis até está tudo correto.
    for (n2 = 0; n2 < gr->nVertices; n2++){         //Pecorre todos os VERTICES.
      if (vetorCusto[n2] != -INFINITY){
        for (n3 = 0; n3 < gr->grau[n2]; n3++){        //Pecorre todas as ARESTAS dos VERTICES.
          if (vetorCusto[gr->arestas[n2][n3]] < pesos[n2][n3]){
            vetorCusto[gr->arestas[n2][n3]] = pesos[n2][n3];
            vetorAnterior[gr->arestas[n2][n3]] = n2;
          }
        }
      }
    }
    
  } 
  printf("Saindo de %d\n", ini);
  for (n1 = 0; n1 < gr->nVertices; n1++){
    printf ("Vertice %d -> vetorCusto: %f\t\t--\t\tvetorAnterior: %f\n", n1, vetorCusto[n1], vetorAnterior[n1]);
  }



}