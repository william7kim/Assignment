// COMP2521 Assignment 1: pagerank.c
//
// Calculate the Page Rank on a couple of metrics
// Number of Outlinks, Dampining Factor, and the max
// Number of iterations.
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "qsort.h"
#include "graph.h"
#include "file.h"

struct order {
    char *url;
    double pageRank;
    int outdegree;
};

static int compare(const void *a, const void *b);

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Not Enough Value!\n");
        exit(EXIT_FAILURE);
    }

    double dampingFactor = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    Graph graph = collectOutgoingURLs();

    // initialize Page Ranks
    int N = numUrl(graph);
    double *prevPR = malloc(N * sizeof(double));
    double *newPR = malloc(N * sizeof(double));
    if (prevPR == NULL || newPR == NULL) {
        fprintf(stderr, "Memory Allocation Failed!\n");
        exit(EXIT_FAILURE);
    }
    double diff = diffPR;
    int iteration = 0;
    for (int i = 0; i < graph->numURL; i++) {
        prevPR[i] = 1.0 / N;
    }
    //start pagerank
    while (iteration < maxIterations && diff >= diffPR) {
        diff = 0.0; 
        for (int j = 0; j < N; j++) {
            int pageIndex = findIndex(graph, findUrlPage(graph, j)); 
            newPR[j] = calcPageRank(graph, pageIndex, dampingFactor, prevPR, j); 
            diff += fabs(newPR[j] - prevPR[j]);
        }
        memcpy(prevPR, newPR, N * sizeof(double));
        iteration++;
    }
    for (int j = 0; j < N; j++) {
        setPageRank(findUrlPage(graph, j), prevPR[j]);
    }
    FILE *pagerankList = fopen("pagerankList.txt","w");
    if (pagerankList == NULL) {
        fprintf(stderr, "pagerankList.txt was not created!\n");
        exit(EXIT_FAILURE);
    }
    struct order array[N];
    for (int i = 0; i < N; i++) {
        array[i].pageRank = graph->Outgoing[i]->pageRank;
        array[i].url = graph->Outgoing[i]->url;
        array[i].outdegree = outDegree(graph, i);
    }
    qsort(array, N, sizeof(struct order), compare);
    for (int i = 0; i < N; i++) {
        fprintf(pagerankList,"%s, %d, %.7f\n", array[i].url, array[i].outdegree, array[i].pageRank);
    }
    fclose(pagerankList);
    free(prevPR); 
    free(newPR);
    // printGraph(graph);
    freeGraph(graph);
    return 0;
}

static int compare(const void *a, const void *b) {
    const struct order *orderA = (const struct order *)a;
    const struct order *orderB = (const struct order *)b;
    // Comparing pageRank first
    if (orderA->pageRank < orderB->pageRank)
        return 1;
    if (orderA->pageRank > orderB->pageRank) 
        return -1;
    // If pageRank is equal, use outdegree 
    if (orderA->outdegree > orderB->outdegree) 
        return -1;
    if (orderA->outdegree < orderB->outdegree) 
        return 1;
    return 0;
}