#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"

typedef struct Graph Graph;

typedef struct {
    char* target; // Etiqueta del nodo destino
    int weight;   // Peso de la conexión
} Edge;

Graph* createGraph();
void destroyGraph(Graph* g);

// Ahora solo recibe el string del label
void addNode(Graph* g, const char* label);

// Las conexiones se hacen directamente usando los labels
void addEdge(Graph* g, const char* src, const char* dest, int weight);

// Consultar adyacentes a través del label. Retorna una List* de Edge*
List* getEdges(Graph* g, const char* label);

List* getAdjacentLabels(Graph* g, const char* label);

int getWeight(Graph* g, const char* label1, const char* label2);

#endif
