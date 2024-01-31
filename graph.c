// COMP2521 Assignment 1: graph.c
//
// A adjacency list implmentation.
// At the bottom of the file includes
// every operation of the Page Rank Algorithm
// called from pagerank.c
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "graph.h"

typedef struct graph *Graph;
typedef struct list *Outgoing;

// first create a 2d array of urls from collection.txt
Graph createGraph(int numURL, char **url) {
	Graph new = malloc(sizeof(struct graph));
	if (new == NULL) {
        fprintf(stderr, "Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
	new->numURL = numURL; 
	new->url = malloc(sizeof(char *) * numURL);
    if (new->url == NULL) {
        fprintf(stderr, "Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numURL; i++) {
        new->url[i] = strdup(url[i]);
        if (new->url[i] == NULL) {
            fprintf(stderr, "Memory Allocation Failed!");
            exit(EXIT_FAILURE);
        }
    }
    new->Outgoing = malloc(sizeof(struct list *) * numURL);
    if (new->Outgoing == NULL) {
        fprintf(stderr, "Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
	for (int i = 0; i < numURL; i++) {
       new->Outgoing[i] = createList(new->url[i]); 
    }
	return new;
}

Outgoing createList(char *url) {
    Outgoing newNode = malloc(sizeof(struct list));
    if (newNode == NULL) {
        fprintf(stderr, "Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
    newNode->url = strdup(url);
    if (newNode->url == NULL) {
        fprintf(stderr, "Memory Allocation Failed!");
        exit(EXIT_FAILURE);
    }
    newNode->pageRank = 0.0;
    newNode->next = NULL;
    return newNode;
}

void freeGraph(Graph graph) {
    if (graph == NULL) 
        return;
    for (int i = 0; i < graph->numURL; i++) {
        free(graph->url[i]);
    }
    free(graph->url);
    for (int i = 0; i < graph->numURL; i++) {
        freeOutgoing(graph->Outgoing[i]);
    }
    free(graph->Outgoing);
    free(graph);
}

void freeOutgoing(Outgoing list) {
    Outgoing curr = list;
    while (curr != NULL) {
        Outgoing tmp = curr;
        curr = curr->next;
        free(tmp->url); 
        free(tmp);
    }
}

int numUrl(Graph graph) {
    if (graph == NULL)
        return 0;
    return graph->numURL;
}

int findIndex(Graph graph, Outgoing listNode) {
    for (int index = 0; index < graph->numURL; index++)
    {
        if (strcmp(graph->url[index], listNode->url) == 0)
            return index;
    }
    return -1;
}

char *urlName(Outgoing listNode) {
    return listNode->url;
}

void addEdge(Graph graph, int srcIndex, Outgoing listNode) {
    if (strcmp(graph->url[srcIndex], listNode->url) == 0)
        return;

    if (graph->Outgoing[srcIndex] == NULL) {
        graph->Outgoing[srcIndex] = listNode;
    } else {
        Outgoing curr = graph->Outgoing[srcIndex];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = listNode;
    }
}

// fast check for pagerank calculation refinment
void printGraph(Graph graph) {
    if (graph == NULL) {
        printf("Graph is empty\n");
        return;
    }
    for (int i = 0; i < graph->numURL; i++) {
        // Print the current node
        printf("%d: ", i);
        Outgoing curr = graph->Outgoing[i];
        printf(" %s %.7lf", curr->url, curr->pageRank);
        for (curr = curr->next; curr != NULL; curr = curr->next)
            printf(" ->%s",curr->url);
        printf("\n");
    }
}

/*
    Page Rank Calculations 
*/

double getPageRank(Outgoing listNode) {
    return listNode->pageRank;
}

void setPageRank(Outgoing listNode, double newPR) {
    if (listNode == NULL)
        return;
    listNode->pageRank = newPR;
}

//get src
Outgoing findUrlPage(Graph graph, int index) {
    if (index >= 0 && index < graph->numURL)
        return graph->Outgoing[index];
    return NULL;
}

/*
    PageRank calculation based on the provided algorithm.
*/

double calcPageRank(Graph graph, int pageIndex, double d, double *prevPR, int j) {
    int N = graph->numURL;
    double sum = 0.0;
    char* targetUrl = graph->url[pageIndex];
    for (int i = 0; i < N; i++) {
        if (i == j) {
            continue;
        }
        if (inLink(graph, targetUrl, i)) {
            sum += prevPR[i] / outDegree(graph, i);
        }
    }
    double pageRank = (1 - d) / (double)N + (d * sum);
    return pageRank;
}

bool inLink(Graph graph, char* targetUrl, int sourceIndex) {
    Outgoing curr = graph->Outgoing[sourceIndex];
    while (curr != NULL) {
        if (strcmp(curr->url, targetUrl) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false; 
}

double outDegree(Graph graph, int index) {
    double count = 0.0;
    Outgoing curr = graph->Outgoing[index];
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    count = count - 1;
    return count > 0 ? count : (double)graph->numURL;
}

 
