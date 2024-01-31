// COMP2521 Assignment 1: searchPagerank.c
//
// A Search Engine that goes through each
// pagerankList.txt and invertedIndex.txt
// to parse and collect information and
// returns sorts urls based off there pageranks
// and number of occurences.
//
// Authors:
// William Kim (z5348193@unsw.edu.au)
//
// Written: 1/2/2024

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "qsort.h"
#include "file.h"

#define LENGTH 10  
#define SIZE 1024  

struct search {
    char *url;
    int count;
    int outDegree;
    double pageRank;
    struct search *next;
};

struct orderSearch {
    char *url;
    int count;
    double pageRank;
    int outDegree;
};

static char **collect(char **url, char *search, int *urlCount, int *capacity);
struct search *insert(struct search *head, char *url);
struct search *change(struct search *head, char *url, double PR, int outDegree);
struct orderSearch *convertToArray(struct search *head, int *length);
static int compare(const void *a, const void *b);

int main(int argc, char **argv) {
    if (argc < 2){
        fprintf(stderr, "Need More Search Terms!\n");
        exit(EXIT_FAILURE);
    }
    char **url = malloc(sizeof(char *) * SIZE);
    if (url == NULL) {
        fprintf(stderr, "Memory Allocation for char url Failed!\n");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    int capacity = SIZE;
    for (int i = 1; i < argc; i++) 
        url = collect(url, argv[i], &count, &capacity);
    struct search *head = NULL;
    for (int i = 0; i < count; i++)
        head = insert(head, url[i]);
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    if (pagerankList == NULL) {
        fprintf(stderr, "Failed to open pagerankList.txt\n");
        exit(EXIT_FAILURE);
    }
    char line[SIZE];
    int length = 0;
    while (fgets(line, SIZE, pagerankList) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        int p = 0;
        char **collection = split(line, " ,", &p);
        if (p >= 3) {
            char *tm;
            double num = strtod(collection[2], &tm);
            int outDegree = atoi(collection[1]);
            head = change(head, collection[0], num, outDegree);
        }
        for (int i = 0; i < p; i++) {
            free(collection[i]);
        }
        free(collection);
        length++;
    }
    fclose(pagerankList);
    struct orderSearch *array = convertToArray(head, &length);
    qsort(array, length, sizeof(struct orderSearch), compare);
    for (int i = 0; i < length; i++)
        fprintf(stdout, "%s\n", array[i].url);
    for (int i = 0; i < length; i++) {
        free(array[i].url);
    }
    free(array);
    while (head != NULL) {
        struct search *temp = head;
        head = head->next;
        free(temp->url);
        free(temp);
    }
    for (int j = 0; j < count; j++) {
        free(url[j]);
    }
    free(url);
    return 0;
}

static char **collect(char **url, char *search, int *urlCount, int *capacity) {
    FILE *invertedIndex = fopen("invertedIndex.txt", "r");
    if (invertedIndex == NULL) {
        fprintf(stderr, "Error reading file!\n");
        exit(EXIT_FAILURE);
    }
    char line[SIZE];
    while (fgets(line, SIZE, invertedIndex) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        int localCount = 0;
        char **collection = split(line, " ", &localCount);
        if (strcmp(collection[0], search) != 0) {
            for (int i = 0; i < localCount; i++) {
                free(collection[i]);
            }
            free(collection);
            continue;
        }
        for (int i = 1; i < localCount; i++) {
            if (*urlCount == *capacity) {
                *capacity *= 2;
                char **newUrl = realloc(url, sizeof(char *) * (*capacity));
                if (newUrl == NULL) {
                    fprintf(stderr, "Reallocation Failed!\n");
                    exit(EXIT_FAILURE);
                }
                url = newUrl;
            }
            if (collection[i][0] != 0 && collection[i][0] != '\n' && collection[i][0] != '\t') {
                url[*urlCount] = strdup(collection[i]);
                if (url[*urlCount] == NULL) {
                    fprintf(stderr, "Str Duplication Failed!\n");
                    exit(EXIT_FAILURE);
                }
                (*urlCount)++;
            }
        }
        for (int i = 0; i < localCount; i++) {
            free(collection[i]);
        }
        free(collection);
    }
    fclose(invertedIndex);
    qsort(url, *urlCount, sizeof(char *), strcompare);
    return url;
}

/*
    Linked List Implmentation that uses recursion
*/

struct search *insert(struct search *head, char *url) {
    if (head == NULL || strcmp(head->url, url) > 0) {
        struct search *newNode = malloc(sizeof(struct search));
        newNode->url = strdup(url);
        newNode->count = 1;
        newNode->outDegree = 0;
        newNode->pageRank = 0.0;
        newNode->next = head;
        return newNode;
    } 
    if (strcmp(head->url, url) == 0) {
        head->count++;
        return head;
    }
    head->next = insert(head->next, url); 
    return head;
}

struct search *change(struct search *head, char *url, double PR, int outDegree) {
    if (head == NULL) {
        return head;
    } else if (strcmp(head->url, url) == 0) {
        head->outDegree = outDegree;
        head->pageRank = PR;
        return head;
    } else {
        // Recursive call for the next node
        change(head->next, url, PR, outDegree);
    }
    return head; 
}

/*
    Qsort for ordering by count then page rank then outdegree
*/

struct orderSearch *convertToArray(struct search *head, int *length) {
    // Count the number of nodes in the linked list
    int count = 0;
    struct search *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    // Allocate memory for the array
    struct orderSearch *array = malloc(count * sizeof(struct orderSearch));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Copy data from linked list to array
    current = head;
    for (int i = 0; i < count; i++) {
        array[i].url = strdup(current->url);  
        array[i].count = current->count;
        array[i].pageRank = current->pageRank;
        array[i].outDegree = current->outDegree;

        current = current->next;
    }
    // Set the length
    *length = count;
    return array;
}

static int compare(const void *a, const void *b) {
    const struct orderSearch *searchA = (const struct orderSearch *)a;
    const struct orderSearch *searchB = (const struct orderSearch *)b;
    // Comparing the number of occurrences first
    if (searchA->count < searchB->count) 
        return 1;
    if (searchA->count > searchB->count) 
        return -1;
    // Comparing pageRank next
    if (searchA->pageRank < searchB->pageRank) 
        return 1;
    if (searchA->pageRank > searchB->pageRank) 
        return -1;
    // If pageRank is equal, use outDegree as tiebreaker
    if (searchA->outDegree < searchB->outDegree) 
        return 1;
    if (searchA->outDegree > searchB->outDegree) 
        return -1;
    return 0;
}