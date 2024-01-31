#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph *Graph;
typedef struct list *Outgoing;

struct list {
    char *url;
    double pageRank;
    struct list *next;
};

struct graph {
    int numURL;
    char **url; // Array of index pointer to struct node
    Outgoing *Outgoing; // Array of outgoing
};

/*
    Graph Adjacency List Reprsentation
*/

Graph createGraph(int numURL, char **url);

Outgoing createList(char *url);

void freeGraph(Graph graph);

void freeOutgoing(Outgoing list);

int numUrl(Graph graph);

int findIndex(Graph graph, Outgoing listNode);

Outgoing findUrlPage(Graph graph, int index);

char *urlName(Outgoing listNode);

void addEdge(Graph graph, int srcIndex, Outgoing listNode);

/*
    Sanity Checks (T__T) or using gdb ./pagerank
*/

void printGraph(Graph graph);

/*
    Page Rank Calculation
*/

double getPageRank(Outgoing listNode);

void setPageRank(Outgoing listNode, double newPR);

double calcPageRank(Graph graph, int pageIndex, double d, double *prevPR, int j);

bool inLink(Graph graph, char* targetUrl, int sourceIndex);
// bool inLink(Graph graph, Outgoing nodeSrc, Outgoing nodeCmp);

double outDegree(Graph graph, int index);

#endif
